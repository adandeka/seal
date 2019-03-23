//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#include "sealTestApp.h"
#include "sealApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "MooseSyntax.h"
#include "ModulesApp.h"

template <>
InputParameters
validParams<sealTestApp>()
{
  InputParameters params = validParams<sealApp>();
  return params;
}

sealTestApp::sealTestApp(InputParameters parameters) : MooseApp(parameters)
{
  sealTestApp::registerAll(
      _factory, _action_factory, _syntax, getParam<bool>("allow_test_objects"));
}

sealTestApp::~sealTestApp() {}

void
sealTestApp::registerAll(Factory & f, ActionFactory & af, Syntax & s, bool use_test_objs)
{
  sealApp::registerAll(f, af, s);
  if (use_test_objs)
  {
    Registry::registerObjectsTo(f, {"sealTestApp"});
    Registry::registerActionsTo(af, {"sealTestApp"});
  }
}

void
sealTestApp::registerApps()
{
  registerApp(sealApp);
  registerApp(sealTestApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
// External entry point for dynamic application loading
extern "C" void
sealTestApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  sealTestApp::registerAll(f, af, s);
}
extern "C" void
sealTestApp__registerApps()
{
  sealTestApp::registerApps();
}
