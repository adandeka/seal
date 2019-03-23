#include "sealApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

template <>
InputParameters
validParams<sealApp>()
{
  InputParameters params = validParams<MooseApp>();
  return params;
}

sealApp::sealApp(InputParameters parameters) : MooseApp(parameters)
{
  sealApp::registerAll(_factory, _action_factory, _syntax);
}

sealApp::~sealApp() {}

void
sealApp::registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  ModulesApp::registerAll(f, af, s);
  Registry::registerObjectsTo(f, {"sealApp"});
  Registry::registerActionsTo(af, {"sealApp"});

  /* register custom execute flags, action syntax, etc. here */
}

void
sealApp::registerApps()
{
  registerApp(sealApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
extern "C" void
sealApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  sealApp::registerAll(f, af, s);
}
extern "C" void
sealApp__registerApps()
{
  sealApp::registerApps();
}
