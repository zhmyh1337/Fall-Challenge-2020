#pragma region Includes
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdarg>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <sstream>
#include <numeric>
#include <functional>
#include <random>
#include <array>
#include <bitset>
#include <chrono>
#include <optional>
#include <cmath>
#pragma endregion

#if defined(_DEBUG) or defined(NDEBUG)
#define LOCAL_MACHINE 1
#else
#define LOCAL_MACHINE 0
#endif

#pragma region Pragmas
#if not LOCAL_MACHINE and 1
#pragma GCC optimize "Ofast,unroll-loops,omit-frame-pointer,inline"
#pragma GCC option("arch=native", "tune=native", "no-zero-upper")
#pragma GCC target("rdrnd", "popcnt", "avx", "bmi2")
#endif
#pragma endregion

#pragma region ChangeableMacros
#define DEBUG_ACTIVE !LOCAL_MACHINE
#define TIMERS_ACTIVE 1
#define ASSERTS_ACTIVE 1
#define ASSERTS_CAUSE_DEBUG_BREAK LOCAL_MACHINE and 1
#define DEBUG_IN_RELEASE 1
#define VECTOR_ASSERTS 1
#define SHOW_TIME_ANYWAY 1
#pragma endregion

using ChronoClock = std::chrono::high_resolution_clock;

#pragma region Constants
#pragma region Dumps
constexpr bool kActionsDump = false;
constexpr bool kTomeProfitDump = false;
constexpr bool kEnemyCastsProfitDump = false;
constexpr bool kPathTraceDump = true;
constexpr bool kFinishVertexDump = true;
constexpr bool kGraphInfoDump = true;
#pragma endregion

#pragma region GameConstants
constexpr size_t kIngredients = 4;
constexpr size_t kInventoryCapacity = 10;
constexpr size_t kBrewsCount = 5;
constexpr size_t kStartCastCount = 4;
constexpr size_t kFullTomeSize = 42;
constexpr size_t kTomeCapacity = 6;
constexpr size_t kBrewsToWin = 6;
#pragma endregion

#pragma region Changable
constexpr bool kShowHelloMessage = true;
constexpr bool kSing = true;
constexpr bool kInformationSing = true;
constexpr const char* kHelloMessage = "Кулити";
constexpr int kStopLearningAfter = 10;
constexpr size_t kMaxCastsCount = kStartCastCount + kFullTomeSize;
constexpr size_t kAllActionsCapacity = kBrewsCount + 2 * kStartCastCount + 2 * kFullTomeSize;
constexpr int kMaxEdgesFromVertex = kMaxCastsCount * (kInventoryCapacity / 2) + 1 + kBrewsCount + kTomeCapacity;
constexpr size_t kMaxGraphVertexListSize = 2000000;
constexpr size_t kMaxLearnsPerPath = 2;
constexpr size_t kGraphVerticesLimit = 40000;
constexpr decltype(std::declval<std::chrono::milliseconds>().count()) kGraphTimeLimit = 25;
constexpr int kGraphCheckTimeLimitPeriod = 50;
constexpr float kIngredientCost[kIngredients] = { 0.5f, 1.0f, 2.5f, 3.5f };
constexpr float kMaxInventoryWorth = kIngredientCost[kIngredients - 1] * kInventoryCapacity;
#pragma endregion
#pragma endregion

using IngredientsContainer = std::array<int8_t, kIngredients>;

#pragma region Globals
std::mt19937 gRng(5);
std::mt19937 gNotDetRng((uint32_t)ChronoClock::now().time_since_epoch().count()); // Not deterministic random (seeded with time).
ChronoClock::time_point gMoveBeginTimePoint;
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
		std::make_pair(IngredientsContainer{-2, -0, -2, -0}, 6),
		std::make_pair(IngredientsContainer{-3, -0, -2, -0}, 7),
		std::make_pair(IngredientsContainer{-0, -0, -4, -0}, 8),
		std::make_pair(IngredientsContainer{-2, -2, -0, -0}, 8),
		std::make_pair(IngredientsContainer{-2, -0, -3, -0}, 8),
		std::make_pair(IngredientsContainer{-3, -2, -0, -0}, 9),
		std::make_pair(IngredientsContainer{-0, -2, -2, -0}, 10),
		std::make_pair(IngredientsContainer{-0, -0, -5, -0}, 10),
		std::make_pair(IngredientsContainer{-2, -0, -0, -2}, 10),
		std::make_pair(IngredientsContainer{-2, -3, -0, -0}, 11),
		std::make_pair(IngredientsContainer{-3, -0, -0, -2}, 11),
		std::make_pair(IngredientsContainer{-0, -4, -0, -0}, 12),
		std::make_pair(IngredientsContainer{-0, -0, -2, -2}, 12),
		std::make_pair(IngredientsContainer{-0, -2, -3, -0}, 12),
		std::make_pair(IngredientsContainer{-0, -3, -2, -0}, 13),
		std::make_pair(IngredientsContainer{-0, -2, -0, -2}, 14),
		std::make_pair(IngredientsContainer{-0, -0, -3, -2}, 14),
		std::make_pair(IngredientsContainer{-2, -0, -0, -3}, 14),
		std::make_pair(IngredientsContainer{-0, -5, -0, -0}, 15),
		std::make_pair(IngredientsContainer{-0, -0, -0, -4}, 16),
		std::make_pair(IngredientsContainer{-0, -0, -2, -3}, 16),
		std::make_pair(IngredientsContainer{-0, -3, -0, -2}, 17),
		std::make_pair(IngredientsContainer{-0, -2, -0, -3}, 18),
		std::make_pair(IngredientsContainer{-0, -0, -0, -5}, 20),
		std::make_pair(IngredientsContainer{-2, -0, -1, -1}, 9),
		std::make_pair(IngredientsContainer{-0, -1, -2, -1}, 12),
		std::make_pair(IngredientsContainer{-1, -2, -0, -1}, 12),
		std::make_pair(IngredientsContainer{-2, -2, -2, -0}, 13),
		std::make_pair(IngredientsContainer{-2, -0, -2, -2}, 15),
		std::make_pair(IngredientsContainer{-2, -2, -0, -2}, 17),
		std::make_pair(IngredientsContainer{-0, -2, -2, -2}, 19),
		std::make_pair(IngredientsContainer{-1, -1, -1, -1}, 12),
		std::make_pair(IngredientsContainer{-3, -1, -1, -1}, 14),
		std::make_pair(IngredientsContainer{-1, -1, -3, -1}, 16),
		std::make_pair(IngredientsContainer{-1, -3, -1, -1}, 18),
		std::make_pair(IngredientsContainer{-1, -1, -1, -3}, 20),
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

			#if ASSERTS_CAUSE_DEBUG_BREAK
			__debugbreak();
			#endif
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
	void Assert(bool assertion, int line, const char* expr, const char* message) override
	{
	}

	void SummarizeAsserts() override
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

#pragma region Utilities
namespace my
{
	#pragma warning(push)
	#pragma warning(disable : 6385)
	#pragma warning(disable : 6386)
	#pragma warning(disable : 26495)
	template<typename T, size_t _capacity>
	class vector
	{
		T data[_capacity];
		size_t sz = 0;

	public:
		using iterator = T*;
		using const_iterator = const T*;
		using value_type = T;

		template <typename... Ts>
		void emplace_back(Ts&&... args)
		{
			#if VECTOR_ASSERTS
			ASSERT(sz < _capacity);
			#endif
			data[sz++] = T(std::forward<Ts>(args)...);
		}

		template <typename... Ts>
		void emplace(size_t pos, Ts&&... args)
		{
			#if VECTOR_ASSERTS
			ASSERT(sz < _capacity);
			#endif
			for (size_t i = sz; i > pos; --i)
				data[i] = data[i - 1];
			data[pos] = T(std::forward<Ts>(args)...);
			sz++;
		}
		
		void resize(size_t newSize)
		{
			#if VECTOR_ASSERTS
			ASSERT(newSize <= _capacity);
			#endif
			sz = newSize;
		}

		void push_back(T&& val)
		{
			emplace_back(std::move(val));
		}

		void push_back(const T& val)
		{
			emplace_back(val);
		}

		void clear()
		{
			sz = 0;
		}

		size_t size() const
		{
			return sz;
		}

		constexpr size_t capacity() const
		{
			return _capacity;
		}

		bool empty() const
		{
			return !size();
		}

		T& front()
		{
			#if VECTOR_ASSERTS
			ASSERT(!empty());
			#endif
			return data[0];
		}

		const T& front() const
		{
			#if VECTOR_ASSERTS
			ASSERT(!empty());
			#endif
			return data[0];
		}

		T& back()
		{
			#if VECTOR_ASSERTS
			ASSERT(!empty());
			#endif
			return data[sz - 1];
		}

		const T& back() const
		{
			#if VECTOR_ASSERTS
			ASSERT(!empty());
			#endif
			return data[sz - 1];
		}

		void pop_back()
		{
			#if VECTOR_ASSERTS
			ASSERT(!empty());
			#endif
			sz--;
		}

		T& operator[](const size_t index)
		{
			#if VECTOR_ASSERTS
			ASSERT(index < sz);
			#endif
			return data[index];
		}

		const T& operator[](const size_t index) const
		{
			#if VECTOR_ASSERTS
			ASSERT(index < sz);
			#endif
			return data[index];
		}

		void erase(const_iterator from)
		{
			sz = from - begin();
		}

		iterator begin()
		{
			return &data[0];
		}

		iterator end()
		{
			return &data[sz - 1] + 1;
		}

		const_iterator begin() const
		{
			return &data[0];
		}

		const_iterator end() const
		{
			return &data[sz - 1] + 1;
		}
	};
	#pragma warning(pop)

	#pragma warning(push)
	#pragma warning(disable : 26495)
	template<class TimeUnit = std::chrono::milliseconds>
	class timer
	{
	public:
		#if TIMERS_ACTIVE
		timer(const char* name)
			: _name(name), _startPoint(ChronoClock::now())
		{
		}

		void time_passed(const char* timeUnitAbbreviation = "ms")
		{
			fprintf(stderr, "Scope \"%s\" used %lld %s.\n",
				_name,
				std::chrono::duration_cast<TimeUnit>(ChronoClock::now() - _startPoint).count(),
				timeUnitAbbreviation
			);
		}
		#else
		timer(const char*)
		{
		}

		void time_passed()
		{
		}
		#endif

	private:
		const char* _name;
		ChronoClock::time_point _startPoint;
	};
	#pragma warning(pop)

	#pragma warning(push)
	#pragma warning(disable : 26495)
	template<typename T, size_t _capacity>
	class queue
	{
		T data[_capacity];
		T* left = data;
		T* right = data;

	public:
		void clear()
		{
			left = right = data;
		}

		template <typename... Ts>
		void emplace(Ts&&... args)
		{
			*right++ = T(std::forward<Ts>(args)...);
		}

		void push(const T& x)
		{
			*right++ = x;
		}

		size_t size() const
		{
			return right - left;
		}

		bool empty() const
		{
			return !size();
		}

		T& front()
		{
			return *left;
		}

		void pop()
		{
			left++;
		}
	};
	#pragma warning(pop)
}
#pragma endregion

#pragma region GameStructures
struct Action
{
	int actionId; // the unique ID of this spell or recipe
	std::string actionType; // in the first league: BREW; later: CAST, OPPONENT_CAST, LEARN, BREW
	IngredientsContainer delta; // by-tier ingredient change
	int price; // the price in rupees if this is a potion
	int tomeIndex; // in the first two leagues: always 0; later: the index in the tome if this is a tome spell, equal to the read-ahead tax
	int taxCount; // in the first two leagues: always 0; later: the amount of taxed tier-0 ingredients you gain from learning this spell
	bool castable; // in the first league: always 0; later: 1 if this is a castable player spell
	bool repeatable; // for the first two leagues: always 0; later: 1 if this is a repeatable player spell

	#pragma warning(push)
	#pragma warning(disable : 26495)
	Action()
	{
	}
	#pragma warning(pop)

	// For debug.
	#if LOCAL_MACHINE and 1
	// Brew.
	Action(int actionId, const IngredientsContainer& delta, int price)
		: actionId(actionId), delta(delta), price(price)
	{
	}

	// Cast.
	Action(int actionId, const IngredientsContainer& delta, int castable, int repeatable)
		: actionId(actionId), delta(delta), castable(castable), repeatable(repeatable)
	{
	}

	// Learn.
	Action(int actionId, const IngredientsContainer& delta, int tomeIndex, int taxCount, int repeatable)
		: actionId(actionId), delta(delta), tomeIndex(tomeIndex), taxCount(taxCount), repeatable(repeatable)
	{
	}
	#endif

	Action(std::istream& in)
	{
		in >> actionId >> actionType >> (int&)delta[0] >> (int&)delta[1] >> (int&)delta[2] >> (int&)delta[3] >>
			price >> tomeIndex >> taxCount >> castable >> repeatable;
	}
};
using AllActionsContainer = my::vector<Action, kAllActionsCapacity>;
using BrewsContainer = my::vector<Action, kBrewsCount>;
using CastsContainer = my::vector<Action, kMaxCastsCount>;
using LearnsContainer = my::vector<Action, kTomeCapacity>;

struct PlayerInfo
{
	IngredientsContainer inv; // by-tier ingredients in inventory
	int score; // amount of rupees

	PlayerInfo() = default;

	PlayerInfo(std::istream& in)
	{
		in >> (int&)inv[0] >> (int&)inv[1] >> (int&)inv[2] >> (int&)inv[3] >> score;
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

	int GetScore() const
	{
		return _lastBalance;
	}

private:
	int _lastBalance = 0;
	int _doneBrews = 0;
} gMyselfTracker, gEnemyTracker;
#pragma endregion

#pragma region SomeDefinitions
namespace Logic
{
	class Graph;
}
namespace DebuggerDump
{
	void GraphInfo(const Logic::Graph&);
}
#pragma endregion

#pragma region Logic
namespace Logic
{
	#pragma region Utilities
	bool CanBrew(const Action& potion, const IngredientsContainer& inv);

	IngredientsContainer operator-(const IngredientsContainer& obj)
	{
		IngredientsContainer result;
		std::transform(obj.begin(), obj.end(), result.begin(), [](IngredientsContainer::value_type x) { return -x; });
		return result;
	}

	int DeltaSum(const IngredientsContainer& arr)
	{
		return std::accumulate(arr.begin(), arr.end(), 0);
	}

	void ApplyDelta(IngredientsContainer& destination, const IngredientsContainer& delta)
	{
		for (size_t i = 0; i < destination.size(); i++)
		{
			destination[i] += delta[i];
		}
	}

	float IngredientsWorth(const IngredientsContainer& ingredients)
	{
		float result = 0.0f;
		for (size_t i = 0; i < ingredients.size(); i++)
		{
			result += ingredients[i] * kIngredientCost[i];
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
	#pragma endregion

	#pragma region Predicates
	bool CanBrew(const Action& brew, const IngredientsContainer& inv)
	{
		bool ok = true;
		for (size_t i = 0; i < inv.size(); i++)
		{
			ok &= brew.delta[i] + inv[i] >= 0;
		}
		return ok;
	}

	bool CanLearn(const Action& learn, const IngredientsContainer& inv)
	{
		return inv[0] >= learn.tomeIndex;
	}

	bool ShouldBeRepeatable(const IngredientsContainer& arr)
	{
		bool ok = false;
		for (const auto x : arr)
		{
			ok |= x < 0;
		}
		return ok;
	}

	bool CorrectInventory(const IngredientsContainer& inv)
	{
		bool ok = true;
		for (const auto x : inv)
		{
			ok &= x >= 0;
		}
		return ok && DeltaSum(inv) <= 10;
	}
	#pragma endregion

	#pragma region Graph
	class Graph
	{
	public:
		enum class EdgeType
		{
			Root = 0,
			Rest,
			Cast,
			Brew,
			Learn
		};
		
		struct VertexInfo;

		struct Edge
		{
			EdgeType edgeType;
			VertexInfo* vertex;
			const Action* performedAction;
			int additionalData; // Cast times.

			#pragma warning(push)
			#pragma warning(disable : 26495)
			Edge()
			{
			}
			#pragma warning(pop)

			Edge(EdgeType edgeType, VertexInfo* vertex = nullptr, const Action* performedAction = nullptr, int additionalData = 0)
				: edgeType(edgeType), vertex(vertex), performedAction(performedAction), additionalData(additionalData)
			{
			}

			Edge(const Edge& other, VertexInfo* newVertex)
				: Edge(other.edgeType, newVertex, other.performedAction, other.additionalData)
			{
			}

			#pragma warning(push)
			#pragma warning(disable : 4715)
			std::string ToString() const
			{
				switch (edgeType)
				{
				case EdgeType::Rest:
					return "REST";
				case EdgeType::Cast:
					return std::string("CAST ") + std::to_string(performedAction->actionId) + " " + std::to_string(additionalData);
				case EdgeType::Brew:
					return std::string("BREW ") + std::to_string(performedAction->actionId);
				case EdgeType::Learn:
					return std::string("LEARN ") + std::to_string(performedAction->actionId);
				default:
					ASSERT(false, "Unknown edge.");
				}
			}
			#pragma warning(pop)
		};

		struct VertexInfo
		{
			using CastableBitset = std::bitset<kMaxCastsCount>;
			using LearnedBitset = std::bitset<kTomeCapacity>;
			using LearnedCastsCastableBitset = std::bitset<kMaxLearnsPerPath>;
			using BrewedBitset = std::bitset<kBrewsCount>;

			Edge reverseEdge;
			IngredientsContainer inventory;
			CastableBitset castable;

			bool isLeaf;

			int depth;
			LearnedBitset learned;
			BrewedBitset brewed;
			int brewCount;
			int minBrewDepth;
			int brewsPrice;
			my::vector<const Action*, kMaxLearnsPerPath> learnedCasts;
			LearnedCastsCastableBitset learnedCastsCastable;

			#pragma warning(push)
			#pragma warning(disable : 26495)
			VertexInfo()
			{
			}
			#pragma warning(pop)

			VertexInfo(const Edge& reverseEdge, const IngredientsContainer& inventory, CastableBitset castable,
				int depth = 0, LearnedBitset learned = 0, BrewedBitset brewed = 0,
				int brewCount = 0, int minBrewDepth = 0, int brewsPrice = 0,
				const decltype(learnedCasts)& learnedCasts = {}, LearnedCastsCastableBitset learnedCastsCastable = 0
			)
				: reverseEdge(reverseEdge), inventory(inventory), castable(castable),
				isLeaf(true),
				depth(depth), learned(learned), brewed(brewed), brewCount(brewCount),
				minBrewDepth(minBrewDepth), brewsPrice(brewsPrice),
				learnedCasts(learnedCasts), learnedCastsCastable(learnedCastsCastable)
			{
			}
		};

		#pragma warning(push)
		#pragma warning(disable : 26495)
		Graph()
		{
		}
		#pragma warning(pop)

		void Initialize(const CastsContainer& casts, const BrewsContainer& brews, const LearnsContainer& learns,
			const IngredientsContainer& inventory)
		{
			_casts = &casts;
			_brews = &brews;
			_learns = &learns;
			_inventory = &inventory;
			vertexList.clear();

			VertexInfo::CastableBitset castable;
			for (size_t i = 0; i < casts.size(); i++)
			{
				castable[i] = casts[i].castable;
			}

			vertexList.emplace_back(Edge(EdgeType::Root), inventory, castable);

			BFS();
		}

	public:
		my::vector<VertexInfo, kMaxGraphVertexListSize> vertexList;

	private:
		void AppendRestVertexOfVertex(my::vector<VertexInfo*, kMaxEdgesFromVertex>& newVerticesList, VertexInfo* vertex)
		{
			Edge reverseEdge(EdgeType::Rest, vertex);
			vertexList.emplace_back(
				reverseEdge, vertex->inventory, (1ll << _casts->size()) - 1, vertex->depth + 1,
				vertex->learned, vertex->brewed, vertex->brewCount, vertex->minBrewDepth,
				vertex->brewsPrice, vertex->learnedCasts, (1ll << vertex->learnedCasts.size()) - 1
			);
			newVerticesList.emplace_back(&vertexList.back());
		}

		void AppendBrewVerticesOfVertex(my::vector<VertexInfo*, kMaxEdgesFromVertex>& newVerticesList, VertexInfo* vertex)
		{
			for (size_t i = 0; i < kBrewsCount; i++)
			{
				if (!vertex->brewed[i] && CanBrew((*_brews)[i], vertex->inventory))
				{
					auto newInventory = vertex->inventory;
					ApplyDelta(newInventory, (*_brews)[i].delta);

					Edge reverseEdge(EdgeType::Brew, vertex, &(*_brews)[i]);
					vertexList.emplace_back(
						reverseEdge, newInventory, vertex->castable, vertex->depth + 1, 
						vertex->learned, vertex->brewed | VertexInfo::BrewedBitset(1ll << i),
						vertex->brewCount + 1, vertex->brewCount ? vertex->minBrewDepth : vertex->depth,
						vertex->brewsPrice + (*_brews)[i].price,
						vertex->learnedCasts, vertex->learnedCastsCastable
					);
					newVerticesList.emplace_back(&vertexList.back());
				}
			}
		}

		void AppendLearnVerticesOfVertex(my::vector<VertexInfo*, kMaxEdgesFromVertex>& newVerticesList, VertexInfo* vertex)
		{
			if (vertex->learnedCasts.size() >= vertex->learnedCasts.capacity())
			{
				return;
			}

			for (size_t i = 0; i < _learns->size(); i++)
			{
				if (!vertex->learned[i] && CanLearn((*_learns)[i], vertex->inventory))
				{
					auto newInventory = vertex->inventory;
					newInventory[0] += (*_learns)[i].taxCount - (*_learns)[i].tomeIndex;
					newInventory[0] -= std::max(0, DeltaSum(newInventory) - (int)kInventoryCapacity);

					auto newLearnedCasts = vertex->learnedCasts;
					newLearnedCasts.emplace_back(&(*_learns)[i]);

					Edge reverseEdge(EdgeType::Learn, vertex, &(*_learns)[i]);
					vertexList.emplace_back(
						reverseEdge, newInventory, vertex->castable, vertex->depth + 1, 
						vertex->learned | VertexInfo::LearnedBitset(1ll << i), vertex->brewed, 
						vertex->brewCount, vertex->minBrewDepth, vertex->brewsPrice,
						newLearnedCasts,
						vertex->learnedCastsCastable | VertexInfo::LearnedCastsCastableBitset(1ll << vertex->learnedCasts.size())
					);
					newVerticesList.emplace_back(&vertexList.back());
				}
			}
		}

		void AppendCastVerticesOfVertex(my::vector<VertexInfo*, kMaxEdgesFromVertex>& newVerticesList, VertexInfo* vertex)
		{
			for (size_t i = 0; i < _casts->size(); i++)
			{
				if (vertex->castable[i])
				{
					auto newInventory = vertex->inventory;
					auto newCastable = vertex->castable ^ VertexInfo::CastableBitset(1ll << i);

					for (int castTimes = 1; (*_casts)[i].repeatable || castTimes == 1; castTimes++)
					{
						ApplyDelta(newInventory, (*_casts)[i].delta);
						if (!CorrectInventory(newInventory))
						{
							break;
						}

						Edge reverseEdge(EdgeType::Cast, vertex, &(*_casts)[i], castTimes);
						vertexList.emplace_back(
							reverseEdge, newInventory, newCastable, vertex->depth + 1, vertex->learned, 
							vertex->brewed, vertex->brewCount, vertex->minBrewDepth, vertex->brewsPrice,
							vertex->learnedCasts, vertex->learnedCastsCastable
						);
						
						newVerticesList.emplace_back(&vertexList.back());
					}
				}
			}

			for (size_t i = 0; i < vertex->learnedCasts.size(); i++)
			{
				if (vertex->learnedCastsCastable[i])
				{
					auto newInventory = vertex->inventory;
					auto newLearnedCastsCastable = vertex->learnedCastsCastable ^ VertexInfo::LearnedCastsCastableBitset(1ll << i);

					for (int castTimes = 1; vertex->learnedCasts[i]->repeatable || castTimes == 1; castTimes++)
					{
						ApplyDelta(newInventory, vertex->learnedCasts[i]->delta);
						if (!CorrectInventory(newInventory))
						{
							break;
						}

						Edge reverseEdge(EdgeType::Cast, vertex, vertex->learnedCasts[i], castTimes);
						vertexList.emplace_back(
							reverseEdge, newInventory, vertex->castable, vertex->depth + 1, vertex->learned,
							vertex->brewed, vertex->brewCount, vertex->minBrewDepth, vertex->brewsPrice,
							vertex->learnedCasts, newLearnedCastsCastable
						);

						newVerticesList.emplace_back(&vertexList.back());
					}
				}
			}
		}

		void AppendAllVerticesOfVertex(my::vector<VertexInfo*, kMaxEdgesFromVertex>& newVerticesList, VertexInfo* vertex)
		{
			AppendRestVertexOfVertex(newVerticesList, vertex);
			AppendBrewVerticesOfVertex(newVerticesList, vertex);
			AppendLearnVerticesOfVertex(newVerticesList, vertex);
			AppendCastVerticesOfVertex(newVerticesList, vertex);
		}

		void BFS()
		{
			static my::queue<VertexInfo*, kMaxGraphVertexListSize> q;
			q.clear();
			q.emplace(&vertexList.front());

			int timerHelperLastCount = 0;
			int timerHelperCounter = 0;

			while (!q.empty())
			{
				auto vertex = q.front();
				q.pop();

				if (vertexList.size() > kGraphVerticesLimit)
				{
					return;
				}

				if (++timerHelperCounter - timerHelperLastCount > kGraphCheckTimeLimitPeriod)
				{
					timerHelperLastCount = timerHelperCounter;
					if (std::chrono::duration_cast<std::chrono::milliseconds>(ChronoClock::now() - gMoveBeginTimePoint).count() >= kGraphTimeLimit)
					{
						return;
					}
				}

				vertex->isLeaf = false;

				my::vector<VertexInfo*, kMaxEdgesFromVertex> newVerticesList;
				newVerticesList.clear();
				AppendAllVerticesOfVertex(newVerticesList, vertex);
				for (auto newVertex : newVerticesList)
				{
					q.emplace(newVertex);
				}
			}
		}

	private:
		const CastsContainer* _casts;
		const BrewsContainer* _brews;
		const LearnsContainer* _learns;
		const IngredientsContainer* _inventory;
	};
	Graph gGraph;
	#pragma endregion

	#pragma region Main
	class NeuralNetwork
	{
	public:
		NeuralNetwork()
		{
// 			neurons = new float*[layersCount];
// 			biasWeights = new float*[layersCount];
// 			weights = new float**[layersCount];
// 
// 			for (size_t i = 0; i < layersCount; i++)
// 			{
// 				neurons[i] = new float[layersSizes[i]];
// 			}
// 
// 			for (size_t i = 1; i < layersCount; i++)
// 			{
// 				biasWeights[i] = new float[layersSizes[i]];
// 			}
// 
// 			for (size_t i = 1; i < layersCount; i++)
// 			{
// 				weights[i] = new float*[layersSizes[i]];
// 				for (size_t j = 0; j < layersSizes[i]; j++)
// 				{
// 					weights[i][j] = new float[layersSizes[i - 1]];
// 				}
// 			}

			SetWeights();
		}

		#if LOCAL_MACHINE
		__declspec(noinline)
		#endif
		void Propagate()
		{
			for (size_t i = 1; i < layersCount; i++)
			{
				for (size_t j = 0; j < layersSizes[i]; j++)
				{
					neurons[i][j] = biasWeights[i][j];
					for (size_t k = 0; k < layersSizes[i - 1]; k++)
					{
						neurons[i][j] += neurons[i - 1][k] * weights[i][j][k];
					}
				}
			}
		}

	private:
		void SetWeights()
		{
			biasWeights[1][0] = -.5479497f;
			biasWeights[1][1] = .103107f;
			biasWeights[1][2] = 1.649468f;
			biasWeights[1][3] = .1152852f;
			biasWeights[1][4] = -.2474401f;
			biasWeights[1][5] = .1843906f;
			biasWeights[1][6] = -.2045283f;
			biasWeights[1][7] = -.8285051f;
			biasWeights[1][8] = -.07187015f;
			biasWeights[1][9] = 1.076015f;
			biasWeights[2][0] = -.3713256f;
			biasWeights[2][1] = -.7967325f;
			biasWeights[2][2] = .347912f;
			biasWeights[2][3] = .7824444f;
			biasWeights[2][4] = .1047546f;
			biasWeights[2][5] = .7434325f;
			biasWeights[2][6] = -.336061f;
			biasWeights[3][0] = -.4476172f;
			biasWeights[3][1] = .9782897f;
			biasWeights[3][2] = .8038584f;
			biasWeights[3][3] = .3365017f;
			biasWeights[3][4] = -.8288228f;
			biasWeights[4][0] = .261638f;

			weights[1][0][0] = .2528998f;
			weights[1][0][1] = -.2662374f;
			weights[1][0][2] = -.01222837f;
			weights[1][0][3] = -.2743579f;
			weights[1][0][4] = .4367718f;
			weights[1][0][5] = .9294345f;
			weights[1][0][6] = -.08072391f;
			weights[1][0][7] = -1.272816f;
			weights[1][0][8] = -.8711374f;
			weights[1][0][9] = -.6624678f;
			weights[1][1][0] = .7891476f;
			weights[1][1][1] = .2053468f;
			weights[1][1][2] = .5710225f;
			weights[1][1][3] = -.1312696f;
			weights[1][1][4] = -.06601107f;
			weights[1][1][5] = .695123f;
			weights[1][1][6] = .6627811f;
			weights[1][1][7] = 1.071865f;
			weights[1][1][8] = .2049464f;
			weights[1][1][9] = -.4021744f;
			weights[1][2][0] = -.1291547f;
			weights[1][2][1] = -.8906752f;
			weights[1][2][2] = -.9906906f;
			weights[1][2][3] = -.1272235f;
			weights[1][2][4] = -.842257f;
			weights[1][2][5] = -1.008027f;
			weights[1][2][6] = -1.014397f;
			weights[1][2][7] = -.1364765f;
			weights[1][2][8] = .4085108f;
			weights[1][2][9] = -.6117935f;
			weights[1][3][0] = -.6357324f;
			weights[1][3][1] = .2672631f;
			weights[1][3][2] = -1.140855f;
			weights[1][3][3] = -1.541782f;
			weights[1][3][4] = -.5533729f;
			weights[1][3][5] = -.2963271f;
			weights[1][3][6] = .6952418f;
			weights[1][3][7] = -.4138355f;
			weights[1][3][8] = .1008788f;
			weights[1][3][9] = -.1775931f;
			weights[1][4][0] = .7070262f;
			weights[1][4][1] = -.004996061f;
			weights[1][4][2] = -.2290836f;
			weights[1][4][3] = -.8827545f;
			weights[1][4][4] = -.6978866f;
			weights[1][4][5] = -1.167688f;
			weights[1][4][6] = .5871309f;
			weights[1][4][7] = -.4952454f;
			weights[1][4][8] = .8309115f;
			weights[1][4][9] = .1865632f;
			weights[1][5][0] = -.946095f;
			weights[1][5][1] = -.008886993f;
			weights[1][5][2] = -.5602939f;
			weights[1][5][3] = .1391259f;
			weights[1][5][4] = -.08962107f;
			weights[1][5][5] = .443424f;
			weights[1][5][6] = -.5919044f;
			weights[1][5][7] = -.8850049f;
			weights[1][5][8] = -.5547545f;
			weights[1][5][9] = -.07270592f;
			weights[1][6][0] = .926011f;
			weights[1][6][1] = -.7177459f;
			weights[1][6][2] = -.4379252f;
			weights[1][6][3] = .4985409f;
			weights[1][6][4] = -1.143897f;
			weights[1][6][5] = -.654201f;
			weights[1][6][6] = -.3103582f;
			weights[1][6][7] = .1844462f;
			weights[1][6][8] = -.6745481f;
			weights[1][6][9] = -.2564582f;
			weights[1][7][0] = .999881f;
			weights[1][7][1] = .3645811f;
			weights[1][7][2] = -.4198595f;
			weights[1][7][3] = -.264191f;
			weights[1][7][4] = -.7037536f;
			weights[1][7][5] = .7802591f;
			weights[1][7][6] = .5807354f;
			weights[1][7][7] = 1.560576f;
			weights[1][7][8] = -.817209f;
			weights[1][7][9] = -.7778218f;
			weights[1][8][0] = -.8852502f;
			weights[1][8][1] = 1.276483f;
			weights[1][8][2] = .1065359f;
			weights[1][8][3] = -.6043265f;
			weights[1][8][4] = .1968153f;
			weights[1][8][5] = -.4627485f;
			weights[1][8][6] = .2925947f;
			weights[1][8][7] = .07892494f;
			weights[1][8][8] = .5806838f;
			weights[1][8][9] = .6244373f;
			weights[1][9][0] = .5372158f;
			weights[1][9][1] = .5125873f;
			weights[1][9][2] = -.8590809f;
			weights[1][9][3] = -.7702805f;
			weights[1][9][4] = -.2055689f;
			weights[1][9][5] = .4968234f;
			weights[1][9][6] = -1.346956f;
			weights[1][9][7] = .2640903f;
			weights[1][9][8] = .7681646f;
			weights[1][9][9] = 1.072824f;
			weights[2][0][0] = .4457051f;
			weights[2][0][1] = -.4586424f;
			weights[2][0][2] = -.5575873f;
			weights[2][0][3] = -.2066777f;
			weights[2][0][4] = .0643239f;
			weights[2][0][5] = -.02271163f;
			weights[2][0][6] = .4046485f;
			weights[2][0][7] = .2789639f;
			weights[2][0][8] = -.2682675f;
			weights[2][0][9] = .02521622f;
			weights[2][1][0] = 1.029627f;
			weights[2][1][1] = -.3099403f;
			weights[2][1][2] = .1499966f;
			weights[2][1][3] = -.9292145f;
			weights[2][1][4] = -.8985267f;
			weights[2][1][5] = -.1911244f;
			weights[2][1][6] = .07662928f;
			weights[2][1][7] = .7353839f;
			weights[2][1][8] = -1.123023f;
			weights[2][1][9] = -.9221045f;
			weights[2][2][0] = .1274f;
			weights[2][2][1] = .7033213f;
			weights[2][2][2] = .9121609f;
			weights[2][2][3] = .005014658f;
			weights[2][2][4] = .7163705f;
			weights[2][2][5] = -.704545f;
			weights[2][2][6] = .165956f;
			weights[2][2][7] = -.3588098f;
			weights[2][2][8] = .9868898f;
			weights[2][2][9] = 1.069103f;
			weights[2][3][0] = -.03275782f;
			weights[2][3][1] = .810928f;
			weights[2][3][2] = -.8048787f;
			weights[2][3][3] = -.7132386f;
			weights[2][3][4] = -.454472f;
			weights[2][3][5] = -.5768313f;
			weights[2][3][6] = .1729208f;
			weights[2][3][7] = -1.018013f;
			weights[2][3][8] = .9295489f;
			weights[2][3][9] = 1.187502f;
			weights[2][4][0] = .2168343f;
			weights[2][4][1] = .9053437f;
			weights[2][4][2] = -.7463222f;
			weights[2][4][3] = -.07068431f;
			weights[2][4][4] = .1533276f;
			weights[2][4][5] = -.1527293f;
			weights[2][4][6] = -1.343978f;
			weights[2][4][7] = .3050795f;
			weights[2][4][8] = -1.308127f;
			weights[2][4][9] = -.4429565f;
			weights[2][5][0] = -.5293161f;
			weights[2][5][1] = -.3832619f;
			weights[2][5][2] = .1065595f;
			weights[2][5][3] = 1.131024f;
			weights[2][5][4] = .4917244f;
			weights[2][5][5] = .8384802f;
			weights[2][5][6] = -.1045517f;
			weights[2][5][7] = .5836747f;
			weights[2][5][8] = .7432402f;
			weights[2][5][9] = .268533f;
			weights[2][6][0] = -.3912694f;
			weights[2][6][1] = -.8073277f;
			weights[2][6][2] = .908623f;
			weights[2][6][3] = .1714838f;
			weights[2][6][4] = .7799314f;
			weights[2][6][5] = -.8951682f;
			weights[2][6][6] = -.5051761f;
			weights[2][6][7] = -.2285018f;
			weights[2][6][8] = .8714608f;
			weights[2][6][9] = .6099778f;
			weights[3][0][0] = .01895487f;
			weights[3][0][1] = -.07053596f;
			weights[3][0][2] = 1.170085f;
			weights[3][0][3] = -.5440374f;
			weights[3][0][4] = -.1950579f;
			weights[3][0][5] = .5356205f;
			weights[3][0][6] = .3884263f;
			weights[3][1][0] = -.1950004f;
			weights[3][1][1] = .6003371f;
			weights[3][1][2] = -.6409447f;
			weights[3][1][3] = -.3317004f;
			weights[3][1][4] = .01784575f;
			weights[3][1][5] = -.8739614f;
			weights[3][1][6] = .4918788f;
			weights[3][2][0] = -.09477806f;
			weights[3][2][1] = -.4168762f;
			weights[3][2][2] = .1734984f;
			weights[3][2][3] = -.5512671f;
			weights[3][2][4] = .7854345f;
			weights[3][2][5] = .6362284f;
			weights[3][2][6] = .4311618f;
			weights[3][3][0] = .8210127f;
			weights[3][3][1] = -.8302097f;
			weights[3][3][2] = -.3405562f;
			weights[3][3][3] = 1.21541f;
			weights[3][3][4] = -.3719326f;
			weights[3][3][5] = -.4208217f;
			weights[3][3][6] = -.06417966f;
			weights[3][4][0] = .741372f;
			weights[3][4][1] = .2998457f;
			weights[3][4][2] = -1.325159f;
			weights[3][4][3] = .8446721f;
			weights[3][4][4] = -.5287423f;
			weights[3][4][5] = -1.30308f;
			weights[3][4][6] = .5380887f;
			weights[4][0][0] = .4907302f;
			weights[4][0][1] = -.3915658f;
			weights[4][0][2] = -.3365276f;
			weights[4][0][3] = -.0585537f;
			weights[4][0][4] = .6068579f;
		}

		static constexpr size_t inputNeuronsCount = 10;
		static constexpr size_t outputNeuronsCount = 1;
		static constexpr size_t hiddenLayersCount = 3;
		static constexpr size_t layersCount = 1 + hiddenLayersCount + 1;
		static constexpr size_t layersSizes[layersCount] = {
			inputNeuronsCount,
			10,
			7,
			5,
			outputNeuronsCount
		};
		static constexpr size_t maxLayerSize = inputNeuronsCount;

		float biasWeights[layersCount][maxLayerSize];
		float weights[layersCount][maxLayerSize][maxLayerSize];
// 		float** biasWeights;
// 		float*** weights;

	public:
		static constexpr size_t inputLayerIndex = 0;
		static constexpr size_t outputLayerIndex = layersCount - 1;

// 		float** neurons;
		float neurons[layersCount][maxLayerSize];

	}gNeuralNetwork;

	namespace Weights
	{
		constexpr float w0 = .6954374f;
		constexpr float w1 = 5.0f;
		constexpr float w2 = .8983427f;
		constexpr float w3 = -1.033479f;
		constexpr float w4 = .8654229f;
		constexpr float w5 = -1.1f;
		constexpr float w6 = .05986932f;
	}

	float InventoryLogWorthEstimation(const IngredientsContainer& inventory)
	{
		return log(IngredientsWorth(inventory) + Weights::w0) * Weights::w1;
	}
	float gInventoryPrecalculatedLogWorthEstimation[kInventoryCapacity + 1][kInventoryCapacity + 1][kInventoryCapacity + 1][kInventoryCapacity + 1];
	float gInventoryPrecalculatedWorthEstimation[kInventoryCapacity + 1][kInventoryCapacity + 1][kInventoryCapacity + 1][kInventoryCapacity + 1];

	float NeuralNetworkEstimate(const Graph::VertexInfo* vertex)
	{
		gNeuralNetwork.neurons[NeuralNetwork::inputLayerIndex][0] = (float)gInventoryPrecalculatedWorthEstimation[vertex->inventory[0]][vertex->inventory[1]][vertex->inventory[2]][vertex->inventory[3]] / 35;
		gNeuralNetwork.neurons[NeuralNetwork::inputLayerIndex][1] = (float)vertex->brewsPrice / 100;
		gNeuralNetwork.neurons[NeuralNetwork::inputLayerIndex][2] = (float)vertex->minBrewDepth / 10;
		gNeuralNetwork.neurons[NeuralNetwork::inputLayerIndex][3] = (float)vertex->brewCount / 6;
		gNeuralNetwork.neurons[NeuralNetwork::inputLayerIndex][4] = (float)vertex->learnedCasts.size() / kMaxCastsCount;
		gNeuralNetwork.neurons[NeuralNetwork::inputLayerIndex][5] = (float)vertex->depth / 10;
		gNeuralNetwork.neurons[NeuralNetwork::inputLayerIndex][6] = (float)gMyselfTracker.GetDoneBrews() / 6;
		gNeuralNetwork.neurons[NeuralNetwork::inputLayerIndex][7] = (float)gMyselfTracker.GetScore() / 150;
		gNeuralNetwork.neurons[NeuralNetwork::inputLayerIndex][8] = (float)gEnemyTracker.GetDoneBrews() / 6;
		gNeuralNetwork.neurons[NeuralNetwork::inputLayerIndex][9] = (float)gMyselfTracker.GetScore() / 150;
		gNeuralNetwork.Propagate();
		return gNeuralNetwork.neurons[NeuralNetwork::outputLayerIndex][0];
	}
	
	float Estimate(const Graph::VertexInfo* vertex)
	{
		return
			+vertex->brewsPrice * Weights::w2
			+vertex->minBrewDepth * Weights::w3
			+vertex->learnedCasts.size() * Weights::w4
			+gInventoryPrecalculatedLogWorthEstimation[vertex->inventory[0]][vertex->inventory[1]][vertex->inventory[2]][vertex->inventory[3]]
			+vertex->depth * Weights::w5
			+vertex->brewCount * Weights::w6
		;
	}

	bool PathsComparator(const Graph::VertexInfo* lhs, const Graph::VertexInfo* rhs)
	{
		#if 1
		return NeuralNetworkEstimate(lhs) < NeuralNetworkEstimate(rhs);
		#else
		return Estimate(lhs) < Estimate(rhs);
		#endif
	}

	auto TracePathToVertex(const Graph::VertexInfo* vertex)
	{
		std::vector<Graph::Edge> result;
		result.reserve(15);
		while (vertex->reverseEdge.edgeType != Graph::EdgeType::Root)
		{
			result.push_back(vertex->reverseEdge);
			vertex = vertex->reverseEdge.vertex;
		}
		std::reverse(result.begin(), result.end());
		return result;
	}

	void FinishVertexDump(const Graph::VertexInfo* vertex)
	{
		if constexpr (kFinishVertexDump && DEBUG)
		{
			dbg.Print("Desired vertex: estimate = %f, brewCount = %d, brewsPrice = %d, minBrewDepth = %d, castable = %d.\n",
				Estimate(vertex), vertex->brewCount, vertex->brewsPrice, vertex->minBrewDepth, vertex->castable.count());
		}
	}

	void PathTraceDump(const std::vector<Graph::Edge>& trace)
	{
		if constexpr (kPathTraceDump && DEBUG)
		{
			dbg.Separator();
			dbg.Print("Path trace:\n");
			
			for (const auto& edge : trace)
			{
				dbg.Print(edge.ToString().c_str());
				dbg.NewLine();
			}

			dbg.Separator();
		}
	}

	float gChoosenVertexEstimation;
	std::string ChooseBestMove()
	{
		static my::vector<const Graph::VertexInfo*, kMaxGraphVertexListSize> vertexBuf;
		vertexBuf.clear();
		for (const auto& vertex : gGraph.vertexList)
		{
			if (vertex.depth)
			{
				vertexBuf.emplace_back(&vertex);
			}
		}

		auto bestVertex = *std::max_element(vertexBuf.begin(), vertexBuf.end(), PathsComparator);

		#if 1
		gChoosenVertexEstimation = NeuralNetworkEstimate(bestVertex);
		#else
		gChoosenVertexEstimation = Estimate(bestVertex);
		#endif

		auto trace = TracePathToVertex(bestVertex);
		FinishVertexDump(bestVertex);
		PathTraceDump(trace);

		return trace.front().ToString();
	}

	std::string DoMain(int moveNumber, PlayerInfo& localInfo, PlayerInfo& enemyInfo, const BrewsContainer& brews,
		const CastsContainer& casts, const CastsContainer& opponent_casts, const LearnsContainer& learns)
	{
		if (gEnemyTracker.UpdateBalance(enemyInfo.score))
		{
			dbg.Print("Enemy brewed a potion.\n");
		}
		if (gMyselfTracker.UpdateBalance(localInfo.score))
		{
			dbg.Print("I brewed a potion.\n");
		}

		my::timer graphInitializeTimer("graph initialize");
		gGraph.Initialize(casts, brews, learns, localInfo.inv);
		graphInitializeTimer.time_passed();
		DebuggerDump::GraphInfo(gGraph);

		my::timer chooseBestMoveTimer("choose best move");
		auto bestMove = ChooseBestMove();
		chooseBestMoveTimer.time_passed();

		return bestMove;
	}
	#pragma endregion
}
#pragma endregion

#pragma region DebuggerDump
namespace DebuggerDump
{
	std::string IngredientsContainerToString(const IngredientsContainer& obj)
	{
		std::ostringstream ss;
		ss << "IngredientsContainer";
		ss << "{ ";
		for (size_t i = 0; i < obj.size(); i++)
		{
			if (i)
			{
				ss << ", ";
			}
			ss << obj[i];
		}
		ss << " }";
		return ss.str();
	}

	void Actions(const BrewsContainer& brews, const CastsContainer& casts, const LearnsContainer& learns, const IngredientsContainer& inventory)
	{
		if constexpr (kActionsDump && DEBUG)
		{
			dbg.Separator();

			for (const auto& brew : brews)
			{
				dbg.Print("brews.emplace_back(%d, %s, %d);\n",
					brew.actionId,
					IngredientsContainerToString(brew.delta).c_str(),
					brew.price
				);
			}

			dbg.NewLine();

			for (const auto& cast : casts)
			{
				dbg.Print("casts.emplace_back(%d, %s, %d, %d);\n",
					cast.actionId,
					IngredientsContainerToString(cast.delta).c_str(),
					cast.castable,
					cast.repeatable
				);
			}

			dbg.NewLine();

			for (const auto& learn : learns)
			{
				dbg.Print("learns.emplace_back(%d, %s, %d, %d, %d);\n",
					learn.actionId,
					IngredientsContainerToString(learn.delta).c_str(),
					learn.tomeIndex,
					learn.taxCount,
					learn.repeatable
				);
			}

			dbg.NewLine();

			dbg.Print("inventory = %s;\n", IngredientsContainerToString(inventory).c_str());

			dbg.Separator();
		}
	}

	template<typename ActionsContainer>
	void Profit(bool assertion, const ActionsContainer& actions, const char* name)
	{
		#if DEBUG
		if (assertion)
		{
			for (auto& it : actions)
			{
				dbg.Print("Profit of %s %d = %f.\n", name, it.actionId, Logic::IngredientsWorth(it.delta));
			}
		}
		#endif
	}

	void GraphInfo(const Logic::Graph& graph)
	{
		#if DEBUG
		if (kGraphInfoDump)
		{
			dbg.Print("Graph size = %llu, max depth = %d.\n", graph.vertexList.size(), graph.vertexList.back().depth);
		}
		#endif
	}
}
#pragma endregion

#pragma region Reading
namespace Reading
{
	template<typename DestinationContainer>
	void ClassifyAction(const AllActionsContainer& source, DestinationContainer& destination, const char* targetName)
	{
		std::copy_if(source.begin(), source.end(), std::back_inserter(destination), [targetName](const Action& obj) {
			return obj.actionType == targetName;
		});
	}

	void ReadActions(BrewsContainer& brews, CastsContainer& casts, CastsContainer& opponent_casts, LearnsContainer& learns)
	{
		int actionCount;
		std::cin >> actionCount;
		AllActionsContainer allActions;
		std::generate_n(std::back_inserter(allActions), actionCount, []() { return Action(std::cin); });

		ClassifyAction(allActions, brews, "BREW");
		ClassifyAction(allActions, casts, "CAST");
		ClassifyAction(allActions, opponent_casts, "OPPONENT_CAST");
		ClassifyAction(allActions, learns, "LEARN");

		ASSERT(allActions.size() == brews.size() + casts.size() + opponent_casts.size() + learns.size(), "some action wasn't recognized");
	}

	void Do(BrewsContainer& brews, CastsContainer& casts, CastsContainer& opponent_casts, LearnsContainer& learns,
		PlayerInfo& localInfo, PlayerInfo& enemyInfo)
	{
		ReadActions(brews, casts, opponent_casts, learns);
		localInfo = PlayerInfo(std::cin);
		enemyInfo = PlayerInfo(std::cin);

		DebuggerDump::Actions(brews, casts, learns, localInfo.inv);
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
		if constexpr (kSing)
		{
			try
			{
				if constexpr (kInformationSing)
				{
					str.append(std::string(" ") + std::to_string(Logic::gChoosenVertexEstimation) + " " +
						std::to_string(Logic::gGraph.vertexList.size()) + " " +
						std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(ChronoClock::now() - gMoveBeginTimePoint).count())
						+ "ms"
					);
				}
				else
				{
					str.append(std::string(" ") + Songs::songsList[Songs::songId].at((moveNumber - kShowHelloMessage - 2) / 1));
				}
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

// Mostly for testing (sandbox).
void Preprocessing()
{
	for (int8_t _1 = 0; _1 <= 10; _1++)
	{
		for (int8_t _2 = 0; _2 <= 10; _2++)
		{
			for (int8_t _3 = 0; _3 <= 10; _3++)
			{
				for (int8_t _4 = 0; _4 <= 10; _4++)
				{
					auto inventory = IngredientsContainer{ _1, _2, _3, _4 };
					if (Logic::CorrectInventory(inventory))
					{
						Logic::gInventoryPrecalculatedWorthEstimation[_1][_2][_3][_4] = Logic::IngredientsWorth(inventory);
						Logic::gInventoryPrecalculatedLogWorthEstimation[_1][_2][_3][_4] = Logic::InventoryLogWorthEstimation(inventory);
					}
				}
			}
		}
	}

	#if LOCAL_MACHINE and 1
	{
		auto startTime = ChronoClock::now();
		constexpr size_t iterationsCount = 100;
		std::vector<int64_t> allTimeDeltas, allGraphTimeDeltas, allBrewHuntDeltas;
		size_t graphSizeSum = 0;
		size_t graphDepthSum = 0;

		for (size_t i = 0; i < iterationsCount; i++)
		{
			gMoveBeginTimePoint = ChronoClock::now();

			BrewsContainer brews;
			CastsContainer casts;
			LearnsContainer learns;
			IngredientsContainer inventory;

// 	 		std::transform(Deck::orders.begin(), Deck::orders.end(), std::back_inserter(brews), [](const auto& obj) {
// 	 			return Action(obj.first, obj.second);
// 	 		});
// 	 		std::shuffle(brews.begin(), brews.end(), gRng);
// 	 		brews.resize(kBrewsCount);
// 	 		std::for_each(brews.begin(), brews.end(), [i=0](Action& brew) mutable {
// 	 			brew.position = brew.actionId = i++;
// 	 		});
// 	 
// 	 		std::transform(Deck::tome.begin(), Deck::tome.end(), std::back_inserter(casts), [](const auto& obj) {
// 	 			return Action(obj, true, true);
// 	 		});
// 	 		std::shuffle(casts.begin(), casts.end(), gRng);
// 	 		casts.resize(kStopLearningAfter - 4);
// 	 		casts.emplace(casts.begin() + 0, IngredientsContainer{ 2, 0, 0, 0 }, true, false);
// 	 		casts.emplace(casts.begin() + 1, IngredientsContainer{ -1, 1, 0, 0 }, true, false);
// 	 		casts.emplace(casts.begin() + 2, IngredientsContainer{ 0, -1, 1, 0 }, true, false);
// 	 		casts.emplace(casts.begin() + 3, IngredientsContainer{ 0, 0, -1, 1 }, true, false);
// 	 
// 	 		inventory = { 1, 1, 1, 1 };

			brews.emplace_back(54, IngredientsContainer{ 0, -2, 0, -2 }, 15);
			brews.emplace_back(64, IngredientsContainer{ 0, 0, -2, -3 }, 19);
			brews.emplace_back(57, IngredientsContainer{ 0, 0, -2, -2 }, 14);
			brews.emplace_back(74, IngredientsContainer{ -3, -1, -1, -1 }, 14);
			brews.emplace_back(52, IngredientsContainer{ -3, 0, 0, -2 }, 11);
			casts.emplace_back(78, IngredientsContainer{ 2, 0, 0, 0 }, 1, 0);
			casts.emplace_back(79, IngredientsContainer{ -1, 1, 0, 0 }, 1, 0);
			casts.emplace_back(80, IngredientsContainer{ 0, -1, 1, 0 }, 1, 0);
			casts.emplace_back(81, IngredientsContainer{ 0, 0, -1, 1 }, 1, 0);
			casts.emplace_back(86, IngredientsContainer{ 2, 1, 0, 0 }, 1, 0);
			casts.emplace_back(88, IngredientsContainer{ -2, 0, -1, 2 }, 1, 1);
			casts.emplace_back(90, IngredientsContainer{ 0, 3, 2, -2 }, 1, 1);
			casts.emplace_back(92, IngredientsContainer{ 0, 2, -1, 0 }, 1, 1);
			casts.emplace_back(93, IngredientsContainer{ -4, 0, 2, 0 }, 1, 1);
			casts.emplace_back(94, IngredientsContainer{ -3, 1, 1, 0 }, 1, 1);
			casts.emplace_back(96, IngredientsContainer{ 1, 2, -1, 0 }, 1, 1);
			learns.emplace_back(17, IngredientsContainer{ -2, 0, 1, 0 }, 0, 1, 1);
			learns.emplace_back(14, IngredientsContainer{ 0, 0, 0, 1 }, 1, 1, 0);
			learns.emplace_back(29, IngredientsContainer{ -5, 0, 0, 2 }, 2, 0, 1);
			learns.emplace_back(6, IngredientsContainer{ 2, 1, -2, 1 }, 3, 0, 1);
			learns.emplace_back(38, IngredientsContainer{ -2, 2, 0, 0 }, 4, 0, 1);
			learns.emplace_back(25, IngredientsContainer{ 0, -3, 0, 2 }, 5, 0, 1);
			inventory = IngredientsContainer{ 1, 1, 0, 0 };

			auto _1 = ChronoClock::now();
			Logic::gGraph.Initialize(casts, brews, learns, inventory);
			allGraphTimeDeltas.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(ChronoClock::now() - _1).count());
			graphSizeSum += Logic::gGraph.vertexList.size();
			graphDepthSum += Logic::gGraph.vertexList.back().depth;

			auto _2 = ChronoClock::now();
			Logic::ChooseBestMove();
			allBrewHuntDeltas.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(ChronoClock::now() - _2).count());
			allTimeDeltas.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(ChronoClock::now() - gMoveBeginTimePoint).count());
		}
		dbg.SummarizeAsserts();
		fprintf(stderr, "Test time: %lld ms.\n", std::chrono::duration_cast<std::chrono::milliseconds>(ChronoClock::now() - startTime).count());
		fprintf(stderr, "Average graph size: %llu.\n", graphSizeSum / iterationsCount);
		fprintf(stderr, "Average graph depth: %f.\n\n", (float)graphDepthSum / iterationsCount);
		fprintf(stderr, "Max time: %lld ms.\n", *std::max_element(allTimeDeltas.begin(), allTimeDeltas.end()));
		fprintf(stderr, "Idmax time: %llu.\n", std::max_element(allTimeDeltas.begin(), allTimeDeltas.end()) - allTimeDeltas.begin());
		fprintf(stderr, "Average time: %f ms.\n\n", (float)std::accumulate(allTimeDeltas.begin(), allTimeDeltas.end(), 0ll) / allTimeDeltas.size());
		fprintf(stderr, "Max graph time: %lld ms.\n", *std::max_element(allGraphTimeDeltas.begin(), allGraphTimeDeltas.end()));
		fprintf(stderr, "Idmax graph time: %llu.\n", std::max_element(allGraphTimeDeltas.begin(), allGraphTimeDeltas.end()) - allGraphTimeDeltas.begin());
		fprintf(stderr, "Average graph time: %f ms.\n\n", (float)std::accumulate(allGraphTimeDeltas.begin(), allGraphTimeDeltas.end(), 0ll) / allGraphTimeDeltas.size());
		fprintf(stderr, "Max choose time: %lld ms.\n", *std::max_element(allBrewHuntDeltas.begin(), allBrewHuntDeltas.end()));
		fprintf(stderr, "Idmax choose time: %llu.\n", std::max_element(allBrewHuntDeltas.begin(), allBrewHuntDeltas.end()) - allBrewHuntDeltas.begin());
		fprintf(stderr, "Average choose time: %f ms.\n\n", (float)std::accumulate(allBrewHuntDeltas.begin(), allBrewHuntDeltas.end(), 0ll) / allBrewHuntDeltas.size());
		#if LOCAL_MACHINE
		exit(0);
// 		__debugbreak();
		#endif
	}	
	#endif
}

int main()
{
	Preprocessing();

	for (int moveNumber = 1; ; moveNumber++)
	{
		BrewsContainer brews;
		CastsContainer casts, opponent_casts;
		LearnsContainer learns;
		PlayerInfo localInfo, enemyInfo;
		Reading::Do(brews, casts, opponent_casts, learns, localInfo, enemyInfo);
		gMoveBeginTimePoint = ChronoClock::now();

		std::string answer = Logic::DoMain(moveNumber, localInfo, enemyInfo, brews, casts, opponent_casts, learns);

		Submitting::Submit(answer, moveNumber);

		#if DEBUG
		dbg.SummarizeAsserts();
		#endif

		#if DEBUG or SHOW_TIME_ANYWAY
		auto timePassed = std::chrono::duration_cast<std::chrono::milliseconds>(ChronoClock::now() - gMoveBeginTimePoint).count();
		static decltype(timePassed) maxTime = 0;
		if (moveNumber == 1)
		{
			fprintf(stderr, "Used %lld ms on the first move.\n", timePassed);
		}
		else
		{
			maxTime = std::max(maxTime, timePassed);
			fprintf(stderr, "Used %lld ms. Max used %lld ms.\n", timePassed, maxTime);
		}
		#endif
	}
}
