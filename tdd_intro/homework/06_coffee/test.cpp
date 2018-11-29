/*
We have to develop a coffee machine with TDD. The machine uses the automated source of ingredients, which implements the interface ISourceOfIngredients.

Our machine should be able to produce the coffee accordingly to the next receipts:
- americano: water & coffee 1:2 or 1:3. Water temp 60C
- cappuccino - milk & coffee & milk foam 1:3, 1:3, 1:3. Water temp 80C
- latte - milk & coffee & milk foam 1:4, 1:2, 1:4. Water temp 90C
- marochino - chocolate & coffee & milk foam, 1:4, 1:4, 1:4 and 1:4 is empty

We have 2 possible sizes of the cup:
- little 100 gram
- big 140 gram

Implement worked coffee machine using ISourceOfIngredients to controll the process of coffee production.
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>

class ISourceOfIngredients
{
public:
    virtual ~ISourceOfIngredients() {}
    virtual void SetCupSize(int gram) = 0;
    virtual void AddWater(int gram, int temperature) = 0;
    virtual void AddSugar(int gram) = 0;
    virtual void AddCoffee(int gram) = 0;
    virtual void AddMilk(int gram) = 0;
    virtual void AddMilkFoam(int gram) = 0;
    virtual void AddChocolate(int gram) = 0;
    virtual void AddCream(int gram) = 0;
};


enum class Cup
{
    Normal,
    Big
};

int GetCupCapacity(const Cup cup)
{
    if (cup == Cup::Big)
    {
        return 140;
    }

    return 100;
}

struct Part
{
    int numerator;
    int denominator;
};

int CalculateQuantity(int value, Part part)
{
    return value * part.numerator / part.denominator;
}

enum class Coffee
{
    Americano,
    Cappuccino,
    Latte
};

class MockSourceOfIngredients : public ISourceOfIngredients
{
public:
    MOCK_METHOD1(SetCupSize, void(int));
    MOCK_METHOD2(AddWater, void(int, int));
    MOCK_METHOD1(AddSugar, void(int));
    MOCK_METHOD1(AddCoffee, void(int));
    MOCK_METHOD1(AddMilk, void(int));
    MOCK_METHOD1(AddMilkFoam, void(int));
    MOCK_METHOD1(AddChocolate, void(int));
    MOCK_METHOD1(AddCream, void(int));
};

class CoffeeMachine
{
public:
    CoffeeMachine(ISourceOfIngredients& source) : m_source(source)
    {

    }
    void CreateCoffee(const Cup cup, const Coffee coffee)
    {
        auto cupCapacity = GetCupCapacity(cup);
        m_source.SetCupSize(cupCapacity);

        if (coffee == Coffee::Cappuccino | coffee == Coffee::Latte)
        {
            m_source.AddMilk(CalculateQuantity(cupCapacity,   Part{1, 3}));
            m_source.AddCoffee(CalculateQuantity(cupCapacity, Part{1, 3}));
            m_source.AddMilk(CalculateQuantity(cupCapacity,   Part{1, 3}));
            return;
        }

        m_source.AddCoffee(CalculateQuantity(cupCapacity, Part{3, 4}));
        m_source.AddWater(CalculateQuantity(cupCapacity,  Part{1, 4}), 60);
    }
private:
    ISourceOfIngredients& m_source;
};


// Architecture
// Class CoffeMachine
// Class-Mock SourceOfIngredients

// - americano: water & coffee 1:2 or 1:3. Water temp 60C

// Tests list:
// 1. americano + 100 gram = 1 coffe
// 2. americano + 140 gram = 1 large coffee
// 3. AddCoffee, SetCupSize and AddWater calls once
// 4. Check parameters
// 5. Same for each recipe

TEST(CoffeeMachine, GetPart1_2ReturnsHalf)
{
    ASSERT_EQ(60, CalculateQuantity(120, Part{1, 2}));
}

TEST(CoffeeMachine, GetPartFloorsResult)
{
    ASSERT_EQ(3, CalculateQuantity(5, Part{2, 3}));
}

TEST(CoffeeMachine, CallsAmericanoIngredientsThings)
{
    using namespace ::testing;

    MockSourceOfIngredients si;
    CoffeeMachine cm(si);

    EXPECT_CALL(si, AddCoffee(_)).Times(1);
    EXPECT_CALL(si, SetCupSize(_)).Times(1);
    EXPECT_CALL(si, AddWater(_, _)).Times(1);

    cm.CreateCoffee(Cup::Normal, Coffee::Americano);
}

//- americano: water & coffee 1:3 Water temp 60C
TEST(CoffeeMachine, CreatesSmallAmericano)
{
    MockSourceOfIngredients si;
    CoffeeMachine cm(si);

    EXPECT_CALL(si, AddCoffee(75)).Times(1);
    EXPECT_CALL(si, SetCupSize(100)).Times(1);
    EXPECT_CALL(si, AddWater(25, 60)).Times(1);

    cm.CreateCoffee(Cup::Normal, Coffee::Americano);
}


//- americano: water & coffee 1:3 Water temp 60C
TEST(CoffeeMachine, CreatesBigAmericano)
{
    MockSourceOfIngredients si;
    CoffeeMachine cm(si);

    EXPECT_CALL(si, AddCoffee(105)).Times(1);
    EXPECT_CALL(si, SetCupSize(140)).Times(1);
    EXPECT_CALL(si, AddWater(35, 60)).Times(1);

    cm.CreateCoffee(Cup::Big, Coffee::Americano);
}

TEST(CoffeeMachine, CallsCappuccinoIngredients)
{
    using namespace ::testing;

    MockSourceOfIngredients si;
    CoffeeMachine cm(si);

    EXPECT_CALL(si, SetCupSize(_)).Times(1);
    EXPECT_CALL(si, AddCoffee(_)).Times(1);
    EXPECT_CALL(si, AddMilk(_)).Times(2);

    cm.CreateCoffee(Cup::Normal, Coffee::Cappuccino);
}

//- americano: water & coffee 1:3 Water temp 60C
TEST(CoffeeMachine, CreatesSmallCappuccino)
{
    MockSourceOfIngredients si;
    CoffeeMachine cm(si);

    EXPECT_CALL(si, SetCupSize(100)).Times(1);
    EXPECT_CALL(si, AddMilk(33)).Times(2);
    EXPECT_CALL(si, AddCoffee(33)).Times(1);

    cm.CreateCoffee(Cup::Normal, Coffee::Cappuccino);
}

//- americano: water & coffee 1:3 Water temp 60C
TEST(CoffeeMachine, CreatesBigCappuccino)
{
    MockSourceOfIngredients si;
    CoffeeMachine cm(si);

    EXPECT_CALL(si, SetCupSize(140)).Times(1);
    EXPECT_CALL(si, AddMilk(46)).Times(2);
    EXPECT_CALL(si, AddCoffee(46)).Times(1);

    cm.CreateCoffee(Cup::Big, Coffee::Cappuccino);
}

TEST(CoffeeMachine, CallsLatteIngredients)
{
    using namespace ::testing;

    MockSourceOfIngredients si;
    CoffeeMachine cm(si);

    EXPECT_CALL(si, SetCupSize(_)).Times(1);
    EXPECT_CALL(si, AddCoffee(_)).Times(1);
    EXPECT_CALL(si, AddMilk(_)).Times(2);

    cm.CreateCoffee(Cup::Normal, Coffee::Latte);
}
