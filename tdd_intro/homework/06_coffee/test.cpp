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
    virtual void AddCoffee(int gram) = 0;
    virtual void AddMilk(int gram) = 0;
    virtual void AddMilkFoam(int gram) = 0;
    virtual void AddChocolate(int gram) = 0;
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
    Latte,
    Marochino
};

class MockSourceOfIngredients : public ISourceOfIngredients
{
public:
    MOCK_METHOD1(SetCupSize, void(int));
    MOCK_METHOD2(AddWater, void(int, int));
    MOCK_METHOD1(AddCoffee, void(int));
    MOCK_METHOD1(AddMilk, void(int));
    MOCK_METHOD1(AddMilkFoam, void(int));
    MOCK_METHOD1(AddChocolate, void(int));
};

class CoffeeMachine
{
public:
    CoffeeMachine(ISourceOfIngredients& source)
        : m_source(source) {}

    void CreateCoffee(const Cup cup, const Coffee coffee)
    {
        auto cupCapacity = GetCupCapacity(cup);
        m_source.SetCupSize(cupCapacity);

        switch (coffee)
        {
            case Coffee::Americano:
            {
                m_source.AddCoffee(CalculateQuantity(cupCapacity, Part{3, 4}));
                m_source.AddWater(CalculateQuantity(cupCapacity,  Part{1, 4}), 60);
                return;
            }
            case Coffee::Cappuccino:
            {
                m_source.AddMilk(CalculateQuantity(cupCapacity,   Part{1, 3}));
                m_source.AddCoffee(CalculateQuantity(cupCapacity, Part{1, 3}));
                m_source.AddMilk(CalculateQuantity(cupCapacity,   Part{1, 3}));
                return;
            }
            case Coffee::Latte:
            {
                m_source.AddMilk(CalculateQuantity(cupCapacity,     Part{1, 4}));
                m_source.AddCoffee(CalculateQuantity(cupCapacity,   Part{1, 2}));
                m_source.AddMilkFoam(CalculateQuantity(cupCapacity, Part{1, 4}));
                return;
            }
            case Coffee::Marochino:
            {
                m_source.AddChocolate(CalculateQuantity(cupCapacity, Part{1, 4}));
                m_source.AddCoffee(CalculateQuantity(cupCapacity,    Part{1, 4}));
                m_source.AddMilk(CalculateQuantity(cupCapacity,      Part{1, 4}));
                return;
            }
        }
    }
private:
    ISourceOfIngredients& m_source;
};


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

// - cappuccino - milk & coffee & milk foam 1:3, 1:3, 1:3. Water temp 80C
TEST(CoffeeMachine, CreatesSmallCappuccino)
{
    MockSourceOfIngredients si;
    CoffeeMachine cm(si);

    EXPECT_CALL(si, SetCupSize(100)).Times(1);
    EXPECT_CALL(si, AddMilk(33)).Times(2);
    EXPECT_CALL(si, AddCoffee(33)).Times(1);

    cm.CreateCoffee(Cup::Normal, Coffee::Cappuccino);
}

// - cappuccino - milk & coffee & milk foam 1:3, 1:3, 1:3. Water temp 80C
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
    EXPECT_CALL(si, AddMilk(_)).Times(1);
    EXPECT_CALL(si, AddMilkFoam(_)).Times(1);
    EXPECT_CALL(si, AddCoffee(_)).Times(1);

    cm.CreateCoffee(Cup::Normal, Coffee::Latte);
}

// - latte - milk & coffee & milk foam 1:4, 1:2, 1:4. Water temp 90C
TEST(CoffeeMachine, CreatesSmallLatte)
{
    MockSourceOfIngredients si;
    CoffeeMachine cm(si);

    EXPECT_CALL(si, SetCupSize(100)).Times(1);
    EXPECT_CALL(si, AddMilk(25)).Times(1);
    EXPECT_CALL(si, AddMilkFoam(25)).Times(1);
    EXPECT_CALL(si, AddCoffee(50)).Times(1);

    cm.CreateCoffee(Cup::Normal, Coffee::Latte);
}

// - latte - milk & coffee & milk foam 1:4, 1:2, 1:4. Water temp 90C
TEST(CoffeeMachine, CreatesBigLatte)
{
    MockSourceOfIngredients si;
    CoffeeMachine cm(si);

    EXPECT_CALL(si, SetCupSize(140)).Times(1);
    EXPECT_CALL(si, AddMilk(35)).Times(1);
    EXPECT_CALL(si, AddMilkFoam(35)).Times(1);
    EXPECT_CALL(si, AddCoffee(70)).Times(1);

    cm.CreateCoffee(Cup::Big, Coffee::Latte);
}

// - marochino - chocolate & coffee & milk foam, 1:4, 1:4, 1:4 and 1:4 is empty
TEST(CoffeeMachine, CallsMarochinoIngredients)
{
    using namespace ::testing;

    MockSourceOfIngredients si;
    CoffeeMachine cm(si);

    EXPECT_CALL(si, SetCupSize(_)).Times(1);
    EXPECT_CALL(si, AddChocolate(_)).Times(1);
    EXPECT_CALL(si, AddCoffee(_)).Times(1);
    EXPECT_CALL(si, AddMilk(_)).Times(1);

    cm.CreateCoffee(Cup::Normal, Coffee::Marochino);
}

// - marochino - chocolate & coffee & milk foam, 1:4, 1:4, 1:4 and 1:4 is empty
TEST(CoffeeMachine, CreatesSmallMarochino)
{
    MockSourceOfIngredients si;
    CoffeeMachine cm(si);

    EXPECT_CALL(si, SetCupSize(100)).Times(1);
    EXPECT_CALL(si, AddChocolate(25)).Times(1);
    EXPECT_CALL(si, AddCoffee(25)).Times(1);
    EXPECT_CALL(si, AddMilk(25)).Times(1);

    cm.CreateCoffee(Cup::Normal, Coffee::Marochino);
}

// - marochino - chocolate & coffee & milk foam, 1:4, 1:4, 1:4 and 1:4 is empty
TEST(CoffeeMachine, CreatesBigMarochino)
{
    MockSourceOfIngredients si;
    CoffeeMachine cm(si);

    EXPECT_CALL(si, SetCupSize(140)).Times(1);
    EXPECT_CALL(si, AddChocolate(35)).Times(1);
    EXPECT_CALL(si, AddCoffee(35)).Times(1);
    EXPECT_CALL(si, AddMilk(35)).Times(1);

    cm.CreateCoffee(Cup::Big, Coffee::Marochino);
}

