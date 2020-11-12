#pragma region Includes
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdarg>
#include <unordered_set>
#include <sstream>
#pragma endregion

#pragma region ChangeableMacros
#define DEBUG_ACTIVE 1
#define ASSERTS_ACTIVE 1
#define DEBUG_IN_RELEASE 1
#pragma endregion

#pragma region Constants
constexpr bool kShowHelloMessage = true;
constexpr const char* kHelloMessage = "Кулити";
constexpr size_t kIngredients = 4;
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

void ReadActions(std::vector<Action>& brews)
{
	int actionCount;
	std::cin >> actionCount;
	auto actions = std::vector<Action>();
	actions.reserve(actionCount);
    std::generate_n(std::back_inserter(actions), actionCount, []() { return Action(std::cin); });

    std::copy_if(actions.begin(), actions.end(), std::back_inserter(brews), [](const Action& obj) { return obj.actionType == "BREW"; });

    ASSERT(actions.size() == brews.size(), "some action wasn't recognized");
}

bool CanBrewPotion(const Action& potion, int inv[])
{
    bool ok = true;
    for (size_t i = 0; i < kIngredients; i++)
    {
        ok &= potion.delta[i] + inv[i] >= 0;
    }
    return true;
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
        ReadActions(brews);
        auto localInfo = PlayerInfo(std::cin);
        auto enemyInfo = PlayerInfo(std::cin);
#pragma endregion

#pragma region Logic
		auto bestPotion = max_element(brews.begin(), brews.end(), [&localInfo](const Action& a, const Action& b) {
			if (!CanBrewPotion(a, localInfo.inv))
				return true;
			if (!CanBrewPotion(b, localInfo.inv))
				return false;
			return a.price < b.price;
		});
#pragma endregion

#pragma region Submitting
        // in the first league: BREW <id> | WAIT; later: BREW <id> | CAST <id> [<times>] | LEARN <id> | REST | WAIT
//         std::cout << "BREW " << bestPotion->actionId << std::endl;
        std::string outputAnswer = std::string("BREW ") + std::to_string(bestPotion->actionId);
        std::cout << AppendHelloMessage(outputAnswer, moveNumber) << std::endl;

        dbg.SummarizeAsserts();
#pragma endregion
    }
}
