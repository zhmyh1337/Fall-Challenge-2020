#pragma region Includes
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdarg>
#include <unordered_set>
#include <sstream>
#include <numeric>
#include <functional>
#pragma endregion

#pragma region ChangeableMacros
#define DEBUG_ACTIVE 1
#define ASSERTS_ACTIVE 1
#define DEBUG_IN_RELEASE 1
#pragma endregion

#pragma region Constants
constexpr bool kActionsDump = false;
constexpr bool kShowHelloMessage = true;
constexpr const char* kHelloMessage = "Кулити";
constexpr size_t kIngredients = 4;
constexpr size_t kInventoryCapacity = 10;
#pragma endregion

#pragma region DEBUGGER
#if DEBUG_ACTIVE and (defined(DEBUG) or DEBUG_IN_RELEASE)
#define DEBUG 1
#else
#define DEBUG 0
#endif

class IDebugger
{
public:
    virtual void Assert(bool assertion, int line, const char* expr, const char* message = nullptr) = 0;
    virtual void SummarizeAsserts() = 0;
    virtual void Print(const char* message, ...) = 0;
    virtual void NewLine() = 0;
    virtual void Separator() = 0;
};

class Debugger : public IDebugger
{
    void Assert(bool assertion, int line, const char* expr, const char* message) override
    {
#if ASSERTS_ACTIVE
        if (!assertion)
        {
            std::ostringstream ss;
            ss << "[Line " << line << "]: " << expr;
            if (message)
            {
                ss << " (" << message << ")";
            }
            ss << ".\n";

            Print(ss.str().c_str());
            _caughtAssertions.insert(std::string("~") + ss.str());
        }
#endif
    }

    void SummarizeAsserts() override
    {
        if (!_caughtAssertions.empty())
        {
            Print("~Caught assertions:\n");
            for (auto& it : _caughtAssertions)
            {
                Print(it.c_str());
            }
        }
    }

    void Print(const char* message, ...) override
    {
        va_list argptr;
        va_start(argptr, message);
        vfprintf(stderr, message, argptr);
        va_end(argptr);
    }

    void NewLine() override
    {
        Print("\n");
    }

    void Separator() override
    {
        Print("========================\n");
    }

private:
    std::unordered_set<std::string> _caughtAssertions;
};

class DummyDebugger : public IDebugger
{
    void Assert(bool assertion, int line, const char* expr, const char* message = nullptr) override
    {
    }
    void Print(const char* message, ...) override
    {
    }
    void NewLine() override
    {
    }
    void Separator() override
    {
    }
    void SummarizeAsserts() override
    {
    }
};

#if DEBUG
    IDebugger& dbg = *new Debugger();
#else
    IDebugger& dbg = *new DummyDebugger();
#endif

#define ASSERT_SHORT(expr) dbg.Assert(!!(expr), __LINE__, #expr)
#define ASSERT_MESSAGE(expr, message) dbg.Assert(!!(expr), __LINE__, #expr, message)
#define ID(x) x
#define GET_MACRO(_1, _2, NAME, ...) NAME
#define ASSERT(...) ID(GET_MACRO(__VA_ARGS__, ASSERT_MESSAGE, ASSERT_SHORT)(__VA_ARGS__))
#pragma endregion

#pragma region UtilitiesStructures

#pragma endregion

#pragma region LogicStructures
struct Action
{
	int actionId; // the unique ID of this spell or recipe
	std::string actionType; // in the first league: BREW; later: CAST, OPPONENT_CAST, LEARN, BREW
    int delta[kIngredients]; // by-tier ingredient change
	int price; // the price in rupees if this is a potion
	int tomeIndex; // in the first two leagues: always 0; later: the index in the tome if this is a tome spell, equal to the read-ahead tax
	int taxCount; // in the first two leagues: always 0; later: the amount of taxed tier-0 ingredients you gain from learning this spell
	bool castable; // in the first league: always 0; later: 1 if this is a castable player spell
	bool repeatable; // for the first two leagues: always 0; later: 1 if this is a repeatable player spell

    Action(std::istream& in)
    {
		in >> actionId >> actionType >> delta[0] >> delta[1] >> delta[2] >> delta[3] >>
            price >> tomeIndex >> taxCount >> castable >> repeatable;
    }
};
#pragma endregion

struct PlayerInfo
{
    int inv[kIngredients]; // by-tier ingredients in inventory
    int score; // amount of rupees

    PlayerInfo(std::istream& in)
    {
        in >> inv[0] >> inv[1] >> inv[2] >> inv[3] >> score;
    }
};

void DumpActions(const std::vector<Action>& actions)
{
    if (kActionsDump)
    {
        for (auto& it : actions)
        {
            dbg.Print("Id: %d, type: %s, price: %d, castable: %d.\n", it.actionId, it.actionType.c_str(), it.price, it.castable);
        }
    }
}

void ReadActions(std::vector<Action>& brews, std::vector<Action>& casts, std::vector<Action>& opponent_casts)
{
	int actionCount;
	std::cin >> actionCount;
	auto actions = std::vector<Action>();
	actions.reserve(actionCount);
    std::generate_n(std::back_inserter(actions), actionCount, []() { return Action(std::cin); });

    DumpActions(actions);

    std::copy_if(actions.begin(), actions.end(), std::back_inserter(brews), [](const Action& obj) { return obj.actionType == "BREW"; });
    std::copy_if(actions.begin(), actions.end(), std::back_inserter(casts), [](const Action& obj) { return obj.actionType == "CAST"; });
    std::copy_if(actions.begin(), actions.end(), std::back_inserter(opponent_casts), [](const Action& obj) { return obj.actionType == "OPPONENT_CAST"; });

    ASSERT(actions.size() == brews.size() + casts.size() + opponent_casts.size(), "some action wasn't recognized");
}

bool CanBrew(const Action& potion, int inv[])
{
    bool ok = true;
    for (size_t i = 0; i < kIngredients; i++)
    {
        ok &= potion.delta[i] + inv[i] >= 0;
    }
    return ok;
}

int VectorSum(const int arr[])
{
	return std::accumulate(arr, arr + kIngredients, 0);
}

bool CanCast(const Action& cast, int inv[])
{
	bool ok = true;
	for (size_t i = 0; i < kIngredients; i++)
	{
		ok &= cast.delta[i] + inv[i] >= 0;
	}
	return ok && cast.castable && VectorSum(inv) + VectorSum(cast.delta) <= kInventoryCapacity;
}

int GetHighestTierMissingPotionIngredient(const Action& potion, int inv[])
{
	ASSERT(!CanBrew(potion, inv), "there must be a missing ingredient");
	for (int i = kIngredients - 1; i >= 0; i--)
	{
		if (inv[i] + potion.delta[i] < 0)
		{
			return i;
		}
	}
	ASSERT(false, "there must be a missing ingredient");
}

std::vector<Action> GetDoableRightNow(const std::vector<Action>& source, int inv[], 
	std::function<bool(const Action& obj, int inv[])> canDo)
{
	auto doableRn = source;
	doableRn.erase(std::remove_if(doableRn.begin(), doableRn.end(), [inv, canDo](const Action& obj) {
		return !canDo(obj, inv);
	}), doableRn.end());
	return doableRn;
}

std::string& AppendHelloMessage(std::string& str, int moveNumber)
{
    if (moveNumber == 1 && kShowHelloMessage)
    {
        str.append(std::string(" ") + kHelloMessage);
    }
    return str;
}

int main()
{
    for (int moveNumber = 1; ; moveNumber++)
    {
#pragma region Reading
        std::vector<Action> brews;
        std::vector<Action> casts;
        std::vector<Action> opponent_casts;
        ReadActions(brews, casts, opponent_casts);
        auto localInfo = PlayerInfo(std::cin);
        auto enemyInfo = PlayerInfo(std::cin);
#pragma endregion
		
#pragma region Logic
		std::string answer = "";

		auto brewableRn = GetDoableRightNow(brews, localInfo.inv, CanBrew);
		auto castableRn = GetDoableRightNow(casts, localInfo.inv, CanCast);

		if (brewableRn.empty())
		{
			const auto& targetPotion = *max_element(brews.begin(), brews.end(), [](const Action& a, const Action& b) {
				return a.price < b.price;
			});

			auto desiredIngredient = GetHighestTierMissingPotionIngredient(targetPotion, localInfo.inv);
			do 
			{
				auto suitableCast = std::find_if(castableRn.begin(), castableRn.end(), [desiredIngredient](const Action& cast) {
					return cast.delta[desiredIngredient] > 0;
				});
				if (suitableCast != castableRn.end())
				{
					answer = std::string("CAST ") + std::to_string(suitableCast->actionId);
					goto submit;
				}
			} while (--desiredIngredient >= 0);
		}
		else
		{
			const auto& bestPotion = *max_element(brewableRn.begin(), brewableRn.end(), [](const Action& a, const Action& b) {
				return a.price < b.price;
			});
			answer = std::string("BREW ") + std::to_string(bestPotion.actionId);
			goto submit;
		}
		answer = "REST";

#pragma endregion
		
#pragma region Submitting
		submit:
        // in the first league: BREW <id> | WAIT; later: BREW <id> | CAST <id> [<times>] | LEARN <id> | REST | WAIT
        std::cout << AppendHelloMessage(answer, moveNumber) << std::endl;

        dbg.SummarizeAsserts();
#pragma endregion
    }
}
