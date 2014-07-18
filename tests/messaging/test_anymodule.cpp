/*
**
** Copyright (C) 2012 Aldebaran Robotics
*/

#include <gtest/gtest.h>

#include <qi/application.hpp>

#include <qi/anyobject.hpp>
#include <qi/session.hpp>
#include <qi/anymodule.hpp>

qi::SessionPtr session;
TEST(Module, Load)
{
  session->loadService("naoqi.testmodule.test");

  qi::AnyObject o = session->service("test");
  ASSERT_TRUE(o);
  int res = o.call<int>("testMethod", 12);
  ASSERT_EQ(13, res);
}

TEST(Module, LoadByHandWithSession)
{
  qi::AnyModule foomod = qi::import("naoqi.testmodulesession");
  qi::AnyObject ao = foomod.call<qi::AnyObject>("Foo", session);
  session->registerService("Foo", ao);
  int res = ao.call<int>("bar");

  ASSERT_EQ(42, res);
}

TEST(Module, LoadWithSessionAndRename)
{
  //## register the Foo object as a service
  session->loadService("naoqi.testmodulesession.Foo", "Bar");

  qi::AnyObject o = session->service("Bar");
  ASSERT_TRUE(o);
  int res = o.call<int>("bar");

  ASSERT_EQ(42, res);
}

int main(int argc, char **argv) {
  qi::Application app(argc, argv);
  ::testing::InitGoogleTest(&argc, argv);
  session = qi::makeSession();

  session->listenStandalone("tcp://127.0.0.1:0");
  return RUN_ALL_TESTS();
}
