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
#include <random>
#include <ctime>
#include <array>
#pragma endregion

#pragma region ChangeableMacros
#define DEBUG_ACTIVE 1
#define ASSERTS_ACTIVE 1
#define DEBUG_IN_RELEASE 1
#pragma endregion

#pragma region Constants
constexpr bool kActionsDump = false;
constexpr bool kShowHelloMessage = true;
constexpr bool kSing = true;
constexpr const char* kHelloMessage = "Кулити";
constexpr size_t kIngredients = 4;
constexpr size_t kInventoryCapacity = 10;
#pragma endregion

#pragma region Usings
using IngredientsContainer = std::array<int, kIngredients>;
#pragma endregion

#pragma region Globals
std::mt19937 gRng(42);
std::mt19937 gNotDetRng((size_t)time(0)); // not deterministic random (seeded with time)
float gIngredientCost[kIngredients] = { 0.5f, 1.0f, 2.5f, 3.5f };
#pragma endregion

#pragma region Hardcode
namespace Deck
{
	std::vector<IngredientsContainer> tome = {
		{-3, 0, 0, 1},
		{3, 0, -1, 0},
		{1, 0, 1, 0},
		{0, 1, 0, 0},
		{3, 0, 0, 0},
		{2, -2, 3, 0},
		{2, -2, 1, 1},
		{3, 1, 0, -1},
		{3, 1, -2, 0},
		{2, 2, -3, 0},
		{2, 0, 2, -1},
		{-4, 2, 0, 0},
		{2, 0, 1, 0},
		{4, 0, 0, 0},
		{0, 0, 0, 1},
		{0, 0, 2, 0},
		{1, 1, 0, 0},
		{-2, 1, 0, 0},
		{-1, 0, -1, 1},
		{0, -1, 2, 0},
		{2, 0, -2, 1},
		{-3, 1, 1, 0},
		{0, -2, 2, 1},
		{1, 1, -3, 1},
		{0, 0, 3, -1},
		{0, 0, -3, 2},
		{1, 1, 1, -1},
		{1, -1, 2, 0},
		{4, -1, 1, 0},
		{-5, 0, 0, 2},
		{-4, 1, 0, 1},
		{0, 2, 3, -2},
		{1, 3, 1, -2},
		{-5, 3, 0, 0},
		{-2, -1, 0, 2},
		{0, -3, 0, 3},
		{0, 3, -3, 0},
		{-3, 0, 3, 0},
		{-2, 0, 2, 0},
		{0, -2, 0, 2},
		{0, 2, -2, 0},
		{0, 2, 0, -1},
	};

	std::vector<std::pair<IngredientsContainer, int>> orders = {
		std::make_pair(IngredientsContainer{2, 0, 2, 0}, 6),
		std::make_pair(IngredientsContainer{3, 0, 2, 0}, 7),
		std::make_pair(IngredientsContainer{0, 0, 4, 0}, 8),
		std::make_pair(IngredientsContainer{2, 2, 0, 0}, 8),
		std::make_pair(IngredientsContainer{2, 0, 3, 0}, 8),
		std::make_pair(IngredientsContainer{3, 2, 0, 0}, 9),
		std::make_pair(IngredientsContainer{0, 2, 2, 0}, 10),
		std::make_pair(IngredientsContainer{0, 0, 5, 0}, 10),
		std::make_pair(IngredientsContainer{2, 0, 0, 2}, 10),
		std::make_pair(IngredientsContainer{2, 3, 0, 0}, 11),
		std::make_pair(IngredientsContainer{3, 0, 0, 2}, 11),
		std::make_pair(IngredientsContainer{0, 4, 0, 0}, 12),
		std::make_pair(IngredientsContainer{0, 0, 2, 2}, 12),
		std::make_pair(IngredientsContainer{0, 2, 3, 0}, 12),
		std::make_pair(IngredientsContainer{0, 3, 2, 0}, 13),
		std::make_pair(IngredientsContainer{0, 2, 0, 2}, 14),
		std::make_pair(IngredientsContainer{0, 0, 3, 2}, 14),
		std::make_pair(IngredientsContainer{2, 0, 0, 3}, 14),
		std::make_pair(IngredientsContainer{0, 5, 0, 0}, 15),
		std::make_pair(IngredientsContainer{0, 0, 0, 4}, 16),
		std::make_pair(IngredientsContainer{0, 0, 2, 3}, 16),
		std::make_pair(IngredientsContainer{0, 3, 0, 2}, 17),
		std::make_pair(IngredientsContainer{0, 2, 0, 3}, 18),
		std::make_pair(IngredientsContainer{0, 0, 0, 5}, 20),
		std::make_pair(IngredientsContainer{2, 0, 1, 1}, 9),
		std::make_pair(IngredientsContainer{0, 1, 2, 1}, 12),
		std::make_pair(IngredientsContainer{1, 2, 0, 1}, 12),
		std::make_pair(IngredientsContainer{2, 2, 2, 0}, 13),
		std::make_pair(IngredientsContainer{2, 0, 2, 2}, 15),
		std::make_pair(IngredientsContainer{2, 2, 0, 2}, 17),
		std::make_pair(IngredientsContainer{0, 2, 2, 2}, 19),
		std::make_pair(IngredientsContainer{1, 1, 1, 1}, 12),
		std::make_pair(IngredientsContainer{3, 1, 1, 1}, 14),
		std::make_pair(IngredientsContainer{1, 1, 3, 1}, 16),
		std::make_pair(IngredientsContainer{1, 3, 1, 1}, 18),
		std::make_pair(IngredientsContainer{1, 1, 1, 3}, 20),
	};
}
#pragma endregion

#pragma region DEBUGGER
#if DEBUG_ACTIVE and (defined(_DEBUG) or DEBUG_IN_RELEASE)
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

#pragma region Songs
std::vector<const char*> gSongs[] = {

	std::vector<const char*>
	{
		"We're no strangers to love",
		"You know the rules and so do I",
		"A full commitment's what I'm thinking of",
		"You wouldn't get this from any other guy",
		"I just wanna tell you how I'm feeling",
		"Gotta make you understand",
		"Never gonna give you up",
		"Never gonna let you down",
		"Never gonna run around and desert you",
		"Never gonna make you cry",
		"Never gonna say goodbye",
		"Never gonna tell a lie and hurt you",
		"We've known each other for so long",
		"Your heart's been aching but you're too shy to say it",
		"Inside we both know what's been going on",
		"We know the game and we're gonna play it",
		"And if you ask me how I'm feeling",
		"Don't tell me you're too blind to see",
		"Never gonna give you up",
		"Never gonna let you down",
		"Never gonna run around and desert you",
		"Never gonna make you cry",
		"Never gonna say goodbye",
		"Never gonna tell a lie and hurt you",
		"Never gonna give you up",
		"Never gonna let you down",
		"Never gonna run around and desert you",
		"Never gonna make you cry",
		"Never gonna say goodbye",
		"Never gonna tell a lie and hurt you",
		"Never gonna give, never gonna give",
		"(Give you up)",
		"(Ooh)Never gonna give, never gonna give",
		"(Give you up)",
		"We've known each other for so long",
		"Your heart's been aching but you're too shy to say it",
		"Inside we both know what's been going on",
		"We know the game and we're gonna play it",
		"I just wanna tell you how I'm feeling",
		"Gotta make you understand",
		"Never gonna give you up",
		"Never gonna let you down",
		"Never gonna run around and desert you",
		"Never gonna make you cry",
		"Never gonna say goodbye",
		"Never gonna tell a lie and hurt you",
		"Never gonna give you up",
		"Never gonna let you down",
		"Never gonna run around and desert you",
		"Never gonna make you cry",
		"Never gonna say goodbye",
		"Never gonna tell a lie and hurt you",
		"Never gonna give you up",
		"Never gonna let you down",
		"Never gonna run around and desert you",
		"Never gonna make you cry",
	},
	std::vector<const char*>
	{
		"Somebody once told me the world is gonna roll me",
		"I ain't the sharpest tool in the shed",
		"She was looking kind of dumb with her finger and her thumb",
		"In the shape of an \"L\" on her forehead",
		"Well, the years start comingand they don't stop coming",
		"Fed to the rules and I hit the ground running",
		"Didn't make sense not to live for fun",
		"Your brain gets smart but your head gets dumb",
		"So much to do, so much to see",
		"So what's wrong with taking the backstreets?",
		"You'll never know if you don't go",
		"You'll never shine if you don't glow",
		"Hey now, you're an all star",
		"Get your game on, go play",
		"Hey now, you're a rock star",
		"Get the show on, get paid",
		"And all that glitters is gold",
		"Only shooting stars break the mold",
		"It's a cool place, and they say it gets colder",
		"You're bundled up now, wait 'til you get older",
		"But the meteor men beg to differ",
		"Judging by the hole in the satellite picture",
		"The ice we skate is getting pretty thin",
		"The water's getting warm so you might as well swim",
		"My world's on fire, how 'bout yours ?",
		"That's the way I like it and I'll never get bored",
		"Hey now, you're an all star",
		"Get your game on, go play",
		"Hey now, you're a rock star",
		"Get the show on, get paid",
		"All that glitters is gold",
		"Only shooting stars break the mold",
		"Somebody once asked",
		"Could I spare some change for gas ?",
		"\"I need to get myself away from this place\"",
		"I said, \"Yep, what a concept",
		"I could use a little fuel myself",
		"And we could all use a little change\"",
		"Well, the years start comingand they don't stop coming",
		"Fed to the rules and I hit the ground running",
		"Didn't make sense not to live for fun",
		"Your brain gets smart but your head gets dumb",
		"So much to do, so much to see",
		"So what's wrong with taking the backstreets?",
		"You'll never know if you don't go(go!)",
		"You'll never shine if you don't glow",
		"Hey now, you're an all star",
		"Get your game on, go play",
		"Hey now, you're a rock star",
		"Get the show on, get paid",
		"And all that glitters is gold",
		"Only shooting stars break the mold",
		"Hey now",
		"Hey now",
		"Hey, hey, hey now",
		"Hey now",
		"Hey now, you're an all star",
		"Hey now, you're an all star",
		"Hey now, you're an all star",
		"Only shooting stars break the mold",
	},
	std::vector<const char*>
	{
		"Yo listen up, here's the story",
		"About a little guy that lives in a blue world",
		"And all day and all night and everything he sees is just blue",
		"Like him, insideand outside",
		"Blue his house with a blue little window",
		"And a blue Corvette",
		"And everything is blue for him",
		"And himselfand everybody around",
		"'Cause he ain't got nobody to listen",
		"I'm blue da ba dee da ba daa",
		"Da ba dee da ba daa, da ba dee da ba daa, da ba dee da ba daa",
		"Da ba dee da ba daa, da ba dee da ba daa, da ba dee da ba daa",
		"I'm blue da ba dee da ba daa",
		"Da ba dee da ba daa, da ba dee da ba daa, da ba dee da ba daa",
		"Da ba dee da ba daa, da ba dee da ba daa, da ba dee da ba daa",
		"I have a blue house with a blue window",
		"Blue is the color of all that I wear",
		"Blue are the streetsand all the trees are too",
		"I have a girlfriendand she is so blue",
		"Blue are the people here that walk around",
		"Blue like my Corvette, it's in and outside",
		"Blue are the words I say and what I think",
		"Blue are the feelings that live inside me",
		"I'm blue da ba dee da ba daa",
		"Da ba dee da ba daa, da ba dee da ba daa, da ba dee da ba daa",
		"Da ba dee da ba daa, da ba dee da ba daa, da ba dee da ba daa",
		"I'm blue da ba dee da ba daa",
		"Da ba dee da ba daa, da ba dee da ba daa, da ba dee da ba daa",
		"Da ba dee da ba daa, da ba dee da ba daa, da ba dee da ba daa",
		"I have a blue house with a blue window",
		"Blue is the color of all that I wear",
		"Blue are the streetsand all the trees are too",
		"I have a girlfriendand she is so blue",
		"Blue are the people here that walk around",
		"Blue like my Corvette, it's in and outside",
		"Blue are the words I say and what I think",
		"Blue are the feelings that live inside me",
		"I'm blue da ba dee da ba daa",
		"Da ba dee da ba daa, da ba dee da ba daa, da ba dee da ba daa",
		"Da ba dee da ba daa, da ba dee da ba daa, da ba dee da ba daa",
		"I'm blue da ba dee da ba daa",
		"Da ba dee da ba daa, da ba dee da ba daa, da ba dee da ba daa",
		"Da ba dee da ba daa, da ba dee da ba daa, da ba dee da ba daa",
	}
};
constexpr auto kSongsCount = sizeof(gSongs) / sizeof(*gSongs);
int gSongId = gNotDetRng() % kSongsCount;
#pragma endregion

#pragma region LogicStructures
struct Action
{
	int actionId; // the unique ID of this spell or recipe
	std::string actionType; // in the first league: BREW; later: CAST, OPPONENT_CAST, LEARN, BREW
    std::array<int, kIngredients> delta; // by-tier ingredient change
	int price; // the price in rupees if this is a potion
	int tomeIndex; // in the first two leagues: always 0; later: the index in the tome if this is a tome spell, equal to the read-ahead tax
	int taxCount; // in the first two leagues: always 0; later: the amount of taxed tier-0 ingredients you gain from learning this spell
	bool castable; // in the first league: always 0; later: 1 if this is a castable player spell
	bool repeatable; // for the first two leagues: always 0; later: 1 if this is a repeatable player spell
	int position = -1; // index in vector

    Action(std::istream& in)
    {
		in >> actionId >> actionType >> delta[0] >> delta[1] >> delta[2] >> delta[3] >>
            price >> tomeIndex >> taxCount >> castable >> repeatable;
    }
};

struct PlayerInfo
{
	std::array<int, kIngredients> inv; // by-tier ingredients in inventory
	int score; // amount of rupees

	PlayerInfo(std::istream& in)
	{
		in >> inv[0] >> inv[1] >> inv[2] >> inv[3] >> score;
	}
};
#pragma endregion

#pragma region Utilities
class Submit
{
public:
	static std::string Brew(const Action& brew, int bonus[2])
	{
		if (brew.position < 2)
		{
			bonusCountdown[brew.position]--;
			if (bonusCountdown[0] <= 0)
			{
				bonusCountdown[0] = bonusCountdown[1];
				bonusCountdown[1] = 0;
				bonus[0] = 1;
				bonus[1] = 0;
			}

			if (bonusCountdown[0] <= 0)
			{
				bonus[0] = 0;
			}
			if (bonusCountdown[1] <= 0)
			{
				bonus[1] = 0;
			}
		}

		return std::string("BREW ") + std::to_string(brew.actionId);
	}
private:
	static int bonusCountdown[2];
};
int Submit::bonusCountdown[2] = { 4, 4 };
#pragma endregion

#pragma region Predicates
bool CanBrew(const Action& potion, const IngredientsContainer& inv)
{
    bool ok = true;
    for (size_t i = 0; i < kIngredients; i++)
    {
        ok &= potion.delta[i] + inv[i] >= 0;
    }
    return ok;
}

int ArraySum(const IngredientsContainer& arr)
{
	return std::accumulate(arr.begin(), arr.end(), 0);
}

bool CheckRepeatable(const IngredientsContainer& arr)
{
	return std::any_of(arr.begin(), arr.end(), [](const int x) { return x < 0; });
}

bool CanCast(const Action& cast, const IngredientsContainer& inv)
{
	bool ok = true;
	for (size_t i = 0; i < kIngredients; i++)
	{
		ok &= cast.delta[i] + inv[i] >= 0;
	}
	return ok && cast.castable && ArraySum(inv) + ArraySum(cast.delta) <= kInventoryCapacity;
}

float CastProfit(const IngredientsContainer& cast)
{
	float result = 0.0f;
	for (size_t i = 0; i < cast.size(); i++)
	{
		result += cast[i] * gIngredientCost[i];
	}
	return result;
}
#pragma endregion

#pragma warning(push)
#pragma warning(disable : 4715)
int GetHighestTierMissingPotionIngredient(const Action& potion, const IngredientsContainer& inv)
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
#pragma warning(pop)

std::vector<Action> GetDoableRightNow(const std::vector<Action>& source, const IngredientsContainer& inv, 
	std::function<bool(const Action& obj, const IngredientsContainer& inv)> canDo)
{
	auto doableRn = source;
	doableRn.erase(std::remove_if(doableRn.begin(), doableRn.end(), [inv, canDo](const Action& obj) {
		return !canDo(obj, inv);
	}), doableRn.end());
	return doableRn;
}

std::string& AppendMessage(std::string& str, int moveNumber)
{
    if (moveNumber == 1 && kShowHelloMessage)
    {
        str.append(std::string(" ") + kHelloMessage);
    }
	if (kSing)
	{
		try
		{
			str.append(std::string(" ") + gSongs[gSongId].at((moveNumber - kShowHelloMessage - 2) / 1));
		}
		catch (...)
		{
		}
	}
    return str;
}

void DumpActions(const std::vector<Action>& actions)
{
	if (kActionsDump)
	{
		for (auto& it : actions)
		{
			dbg.Print("Id: %d, t: %s, p: %d, c: %d, r: %d.\n", it.actionId, it.actionType.c_str(),
				it.price, it.castable, it.repeatable);
		}
	}
}

void ClassifyAction(const std::vector<Action>& source, std::vector<Action>& destination, const char* targetName)
{
	std::copy_if(source.begin(), source.end(), std::back_inserter(destination), [targetName](const Action& obj) {
		return obj.actionType == targetName;
	});
	for (size_t i = 0; i < destination.size(); i++)
	{
		destination[i].position = i;
	}
}

void ReadActions(std::vector<Action>& brews, std::vector<Action>& casts, std::vector<Action>& opponent_casts, std::vector<Action>& learns)
{
	int actionCount;
	std::cin >> actionCount;
	auto actions = std::vector<Action>();
	actions.reserve(actionCount);
	std::generate_n(std::back_inserter(actions), actionCount, []() { return Action(std::cin); });

	DumpActions(actions);

	ClassifyAction(actions, brews, "BREW");
	ClassifyAction(actions, casts, "CAST");
	ClassifyAction(actions, opponent_casts, "OPPONENT_CAST");
	ClassifyAction(actions, learns, "LEARN");

	ASSERT(actions.size() == brews.size() + casts.size() + opponent_casts.size() + learns.size(), "some action wasn't recognized");
	ASSERT(brews.size() == 5);
}

int main()
{
	auto tomeProfit = std::vector<std::pair<float, IngredientsContainer>>(Deck::tome.size());
	for (size_t i = 0; i < tomeProfit.size(); i++)
	{
		tomeProfit[i] = std::make_pair(CastProfit(Deck::tome[i]), Deck::tome[i]);
	}

	std::sort(tomeProfit.begin(), tomeProfit.end(), [](const auto& lhs, const auto& rhs) {
		return lhs.first < rhs.first;
	});

	dbg.Print("Preprocessing used %f ms.\n", 1000.0f * clock() / CLOCKS_PER_SEC);
    for (int moveNumber = 1; ; moveNumber++)
    {
#if DEBUG
		clock_t startTime = clock();
#endif

#pragma region Reading
        std::vector<Action> brews;
        std::vector<Action> casts;
        std::vector<Action> opponent_casts;
        std::vector<Action> learns;
        ReadActions(brews, casts, opponent_casts, learns);
        auto localInfo = PlayerInfo(std::cin);
        auto enemyInfo = PlayerInfo(std::cin);
#pragma endregion
		
#pragma region Logic
		std::string answer = "";

		static int bonus[2] = { 3, 1 };
		dbg.Print("Bonuses: %d %d.\n", bonus[0], bonus[1]);
		brews[0].price += bonus[0];
		brews[1].price += bonus[1];

		auto brewableRn = GetDoableRightNow(brews, localInfo.inv, CanBrew);
		auto castableRn = GetDoableRightNow(casts, localInfo.inv, CanCast);

		std::sort(brews.begin(), brews.end(), [](const Action& lhs, const Action& rhs) {
			// Descending!
			return ArraySum(lhs.delta) > ArraySum(rhs.delta);
		});

		const auto& targetPotion = *max_element(brews.begin(), brews.end(), [](const Action& a, const Action& b) {
			return a.price < b.price;
		});

		if (!learns.empty())
		{
			answer = std::string("LEARN ") + std::to_string(learns.front().actionId);
			goto submit;
		}

		if (!CanBrew(targetPotion, localInfo.inv))
		{
			auto desiredIngredient = GetHighestTierMissingPotionIngredient(targetPotion, localInfo.inv);
			do
			{
				auto suitableCasts = castableRn;
				suitableCasts.erase(std::remove_if(suitableCasts.begin(), suitableCasts.end(), [desiredIngredient](const Action& cast) {
					return cast.delta[desiredIngredient] == 0;
				}), suitableCasts.end());

				if (!suitableCasts.empty())
				{
					const auto& bestSuitableCast = *std::max_element(suitableCasts.begin(), suitableCasts.end(),
						[](const Action& lhs, const Action& rhs) {
							return CastProfit(lhs.delta) < CastProfit(rhs.delta);
						}
					);
					answer = std::string("CAST ") + std::to_string(bestSuitableCast.actionId) + " 1";
					goto submit;
				}
			} while (--desiredIngredient >= 0);
		}
		else
		{
			answer = Submit::Brew(targetPotion, bonus);
			goto submit;
		}
		answer = "REST";

#pragma endregion
		
#pragma region Submitting
		submit:
        // in the first league: BREW <id> | WAIT; later: BREW <id> | CAST <id> [<times>] | LEARN <id> | REST | WAIT
        std::cout << AppendMessage(answer, moveNumber) << std::endl;

#if DEBUG
        dbg.SummarizeAsserts();
		float curTime = 1000.0f * (clock() - startTime) / CLOCKS_PER_SEC;
		static float maxTime = 0.0;
		maxTime = std::max(maxTime, curTime);
		dbg.Print("Used %f ms. Max used %f ms.\n", curTime, maxTime);
#endif
#pragma endregion
    }
}
