//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#ifndef SEALTESTAPP_H
#define SEALTESTAPP_H

#include "MooseApp.h"

class sealTestApp;

template <>
InputParameters validParams<sealTestApp>();

class sealTestApp : public MooseApp
{
public:
  sealTestApp(InputParameters parameters);
  virtual ~sealTestApp();

  static void registerApps();
  static void registerAll(Factory & f, ActionFactory & af, Syntax & s, bool use_test_objs = false);
};

#endif /* SEALTESTAPP_H */
