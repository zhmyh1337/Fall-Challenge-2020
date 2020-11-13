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
constexpr bool kTomeProfitDump = false;
constexpr bool kEnemyCastsProfitDump = false;
constexpr bool kShowHelloMessage = true;
constexpr bool kSing = true;
constexpr const char* kHelloMessage = "Кулити";
constexpr size_t kIngredients = 4;
constexpr size_t kInventoryCapacity = 10;
constexpr int kStopLearningAfter = 11;
#pragma endregion

using IngredientsContainer = std::array<int, kIngredients>;

#pragma region Globals
std::mt19937 gRng(42);
std::mt19937 gNotDetRng((uint32_t)time(0)); // not deterministic random (seeded with time)
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

#pragma region Debugger
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
namespace Songs
{
	std::vector<const char*> songsList[] = {

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
	constexpr auto songsCount = sizeof(songsList) / sizeof(*songsList);
	int songId = gNotDetRng() % songsCount;
}
#pragma endregion

#pragma region GameStructures
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
using ActionsContainer = std::vector<Action>;

struct PlayerInfo
{
	std::array<int, kIngredients> inv; // by-tier ingredients in inventory
	int score; // amount of rupees

	PlayerInfo() = default;

	PlayerInfo(std::istream& in)
	{
		in >> inv[0] >> inv[1] >> inv[2] >> inv[3] >> score;
	}
};

class EnemyTracker
{
public:
	bool UpdateBalance(int newBalance)
	{
		if (_lastBalance != newBalance)
		{
			_doneBrews++;
			_lastBalance = newBalance;
			return true;
		}
		_lastBalance = newBalance;
		return false;
	}

	int GetDoneBrews() const
	{
		return _doneBrews;
	}

private:
	int _lastBalance = 0;
	int _doneBrews = 0;
};
#pragma endregion

#pragma region Logic
namespace Logic
{
	#pragma region Utilities
	bool CanBrew(const Action& potion, const IngredientsContainer& inv);

	int ArraySum(const IngredientsContainer& arr)
	{
		return std::accumulate(arr.begin(), arr.end(), 0);
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

	ActionsContainer GetDoableRightNow(const ActionsContainer& source, const IngredientsContainer& inv,
		std::function<bool(const Action& obj, const IngredientsContainer& inv)> canDo)
	{
		auto doableRn = source;
		doableRn.erase(std::remove_if(doableRn.begin(), doableRn.end(), [inv, canDo](const Action& obj) {
			return !canDo(obj, inv);
			}), doableRn.end());
		return doableRn;
	}
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

	bool ShouldBeRepeatable(const IngredientsContainer& arr)
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
	#pragma endregion

	#pragma region Logic
	std::string Do(int moveNumber, PlayerInfo& localInfo, PlayerInfo& enemyInfo, const ActionsContainer& brews, 
		const ActionsContainer& casts, const ActionsContainer& opponent_casts, const ActionsContainer& learns)
	{
		static auto enemyTracker = EnemyTracker();
		static auto myselfTracker = EnemyTracker();
		if (enemyTracker.UpdateBalance(enemyInfo.score))
		{
			dbg.Print("Enemy brewed a potion.\n");
		}
		if (myselfTracker.UpdateBalance(localInfo.score))
		{
			dbg.Print("I brewed a potion.\n");
		}

		auto brewableRn = GetDoableRightNow(brews, localInfo.inv, CanBrew);
		auto castableRn = GetDoableRightNow(casts, localInfo.inv, CanCast);

		auto sortedBrews = brews;
		std::sort(sortedBrews.begin(), sortedBrews.end(), [](const Action& lhs, const Action& rhs) {
			// Descending!
			return ArraySum(lhs.delta) > ArraySum(rhs.delta);
		});

		const auto& targetPotion = *max_element(sortedBrews.begin(), sortedBrews.end(), [](const Action& a, const Action& b) {
			return a.price < b.price;
		});

		if (!learns.empty() && myselfTracker.GetDoneBrews() >= enemyTracker.GetDoneBrews())
		{
			return std::string("LEARN ") + std::to_string(learns.front().actionId);
		}

		dbg.Print("Target potion: %d (%d).\n", targetPotion.actionId, targetPotion.price);
		if (!CanBrew(targetPotion, localInfo.inv))
		{
			auto desiredIngredient = GetHighestTierMissingPotionIngredient(targetPotion, localInfo.inv);
			dbg.Print("Desired ingredient: %d.\n", desiredIngredient);
			do
			{
				auto suitableCasts = castableRn;
				suitableCasts.erase(std::remove_if(suitableCasts.begin(), suitableCasts.end(), [desiredIngredient](const Action& cast) {
					return cast.delta[desiredIngredient] <= 0;
					}), suitableCasts.end());

				if (!suitableCasts.empty())
				{
					const auto& bestSuitableCast = *std::max_element(suitableCasts.begin(), suitableCasts.end(),
						[](const Action& lhs, const Action& rhs) {
							return CastProfit(lhs.delta) < CastProfit(rhs.delta);
						}
					);
					return std::string("CAST ") + std::to_string(bestSuitableCast.actionId) + " 1";
				}
			} while (--desiredIngredient >= 0);
		}
		else
		{
			return std::string("BREW ") + std::to_string(targetPotion.actionId);
		}

		return "REST";
	}
	#pragma endregion
}
#pragma endregion

#pragma region DebuggerDump
namespace DebuggerDump
{
	void Actions(const ActionsContainer& actions)
	{
		#if DEBUG
		if (kActionsDump)
		{
			for (auto& it : actions)
			{
				dbg.Print("Id: %d, t: %s, p: %d, c: %d, r: %d.\n", it.actionId, it.actionType.c_str(),
					it.price, it.castable, it.repeatable);
			}
		}
		#endif
	}

	void Profit(bool assertion, const ActionsContainer& actions, const char* name)
	{
		#if DEBUG
		if (assertion)
		{
			for (auto& it : actions)
			{
				dbg.Print("Profit of %s %d = %f.\n", name, it.actionId, Logic::CastProfit(it.delta));
			}
		}
		#endif
	}
}
#pragma endregion

#pragma region Reading
namespace Reading
{
	void ClassifyAction(const ActionsContainer& source, ActionsContainer& destination, const char* targetName)
	{
		std::copy_if(source.begin(), source.end(), std::back_inserter(destination), [targetName](const Action& obj) {
			return obj.actionType == targetName;
		});
		for (size_t i = 0; i < destination.size(); i++)
		{
			destination[i].position = i;
		}
	}

	void ReadActions(ActionsContainer& brews, ActionsContainer& casts, ActionsContainer& opponent_casts, ActionsContainer& learns)
	{
		int actionCount;
		std::cin >> actionCount;
		auto actions = ActionsContainer();
		actions.reserve(actionCount);
		std::generate_n(std::back_inserter(actions), actionCount, []() { return Action(std::cin); });

		DebuggerDump::Actions(actions);

		ClassifyAction(actions, brews, "BREW");
		ClassifyAction(actions, casts, "CAST");
		ClassifyAction(actions, opponent_casts, "OPPONENT_CAST");
		ClassifyAction(actions, learns, "LEARN");

		ASSERT(actions.size() == brews.size() + casts.size() + opponent_casts.size() + learns.size(), "some action wasn't recognized");
		ASSERT(brews.size() == 5);
	}

	void Do(ActionsContainer& brews, ActionsContainer& casts, ActionsContainer& opponent_casts, ActionsContainer& learns,
		PlayerInfo& localInfo, PlayerInfo& enemyInfo)
	{
		ReadActions(brews, casts, opponent_casts, learns);
		localInfo = PlayerInfo(std::cin);
		enemyInfo = PlayerInfo(std::cin);

		DebuggerDump::Profit(kTomeProfitDump, learns, "tome");
		DebuggerDump::Profit(kEnemyCastsProfitDump, opponent_casts, "enemy cast");
	}
}
#pragma endregion

#pragma region Submitting
namespace Submitting
{
	std::string AppendMessage(std::string&& str, int moveNumber)
	{
		if (moveNumber == 1 && kShowHelloMessage)
		{
			str.append(std::string(" ") + kHelloMessage);
		}
		if (kSing)
		{
			try
			{
				str.append(std::string(" ") + Songs::songsList[Songs::songId].at((moveNumber - kShowHelloMessage - 2) / 1));
			}
			catch (...)
			{
			}
		}
		return str;
	}

	void Submit(std::string& command, int moveNumber)
	{
		std::cout << AppendMessage(std::move(command), moveNumber) << std::endl;
	}
}
#pragma endregion

int main()
{
	for (int moveNumber = 1; ; moveNumber++)
	{
		#if DEBUG
		clock_t startTime = moveNumber == 1 ? 0 : clock();
		#endif

		ActionsContainer brews, casts, opponent_casts, learns;
		PlayerInfo localInfo, enemyInfo;
		Reading::Do(brews, casts, opponent_casts, learns, localInfo, enemyInfo);

		std::string answer = Logic::Do(moveNumber, localInfo, enemyInfo, brews, casts, opponent_casts, learns);

		Submitting::Submit(answer, moveNumber);

		#if DEBUG
		dbg.SummarizeAsserts();
		float curTime = 1000.0f * (clock() - startTime) / CLOCKS_PER_SEC;
		static float maxTime = 0.0;
		if (moveNumber == 1)
		{
			dbg.Print("Used %f ms on the first move.\n", curTime);
		}
		else
		{
			maxTime = std::max(maxTime, curTime);
			dbg.Print("Used %f ms. Max used %f ms.\n", curTime, maxTime);
		}
		#endif
	}
}
