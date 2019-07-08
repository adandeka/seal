//* This file is written on basis of the MOOSE framework
//* File written by Akshay Dandekar, contact: adandeka@purdue.edu
//*
#include "KelvinVoigtViscousHeatSource.h"

registerMooseObject("HeatConductionApp", KelvinVoigtViscousHeatSource);

template <>
InputParameters
validParams<KelvinVoigtViscousHeatSource>()
{
  InputParameters params = validParams<HeatSource>();
  params.addClassDescription("Viscous damping heat source");
  return params;
}

KelvinVoigtViscousHeatSource::KelvinVoigtViscousHeatSource(const InputParameters & parameters)
  : HeatSource(parameters),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _stress(getMaterialPropertyByName<RankTwoTensor>("stress")),
    _heat_source_rate(getMaterialProperty<Real>("heat_source_rate")),
    _ndisp(coupledComponents("displacements")),
    _disp_var(_ndisp)
{
  for (unsigned int i = 0; i < _ndisp; ++i)
    _disp_var[i] = coupled("displacements", i);
}

Real
KelvinVoigtViscousHeatSource::computeQpResidual()
{
  return -_heat_source_rate[_qp] * _test[_i][_qp];
}

Real
KelvinVoigtViscousHeatSource::computeQpJacobian()
{
  return -0.0 * _phi[_j][_qp] * _test[_i][_qp];
}

Real
KelvinVoigtViscousHeatSource::computeQpOffDiagJacobian(unsigned int jvar)
{
  Real val;
  unsigned int i, j, k, m;
  val = 0.0;
  if ( _heat_source_rate[_qp] > 0.0 ) {
    for (unsigned int k = 0; k < _ndisp; ++k) {
      if (jvar == _disp_var[k]) {
        for (unsigned int m = 0; m < 2; ++m) {
          val += _stress[_qp](k,m) * _grad_phi[_j][_qp](m) * _test[_i][_qp] / _dt ;
        }
      }
    }
  }
  return val;
}
