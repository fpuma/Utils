#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <utils/containers/uniquerealizationcontainer.h>
#include <utils/containers/mappedrealizationcontainer.h>

using namespace puma;

class ITestBase
{
public:
    virtual ~ITestBase() {}
};

class ITestInterface : public ITestBase
{
public:
    virtual ~ITestInterface() {}
    virtual int get() const = 0;
    virtual void set( int _num ) = 0;
};

class TestRealization0 : public ITestInterface
{
public:

    int get() const override { return m_num; }
    void set( int _num ) override { m_num = _num; }

private:
    int m_num = 0;
};

class TestRealization1 : public ITestBase
{
public:

    float get() const { return m_num; }
    void set( float _num ) { m_num = _num; }

private:
    float m_num = 0;
};

class TestContainerCallbacks
{
public:
    void onAdded() {}
    void onRemoved() {}
};

class TestContainerCallbacksMock : public TestContainerCallbacks
{
public:
    MOCK_METHOD( void, onAdded, () );
    MOCK_METHOD( void, onRemoved, () );
};

class TestUniqueRealizationContainer : public UniqueRealizationContainer<ITestBase>
{
public:

    TestContainerCallbacksMock MockTest;

protected:

    void onAdded( std::shared_ptr<ITestBase> _system ) override { MockTest.onAdded(); }
    void onRemoved( std::shared_ptr<ITestBase> _system ) override { MockTest.onRemoved(); }
};