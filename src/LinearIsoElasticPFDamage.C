/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#include "LinearIsoElasticPFDamage.h"
#include "libmesh/utility.h"

registerMooseObject("TensorMechanicsApp", LinearIsoElasticPFDamage);

template<>
InputParameters validParams<LinearIsoElasticPFDamage>()
{
  InputParameters params = validParams<ComputeStressBase>();
  params.addClassDescription("Phase-field fracture model energy contribution to damage growth-isotropic elasticity and undamaged stress under compressive strain");
  params.addRequiredCoupledVar("c","Order parameter for damage");
  params.addParam<Real>("kdamage",1e-6,"Stiffness of damaged matrix");
  return params;
}

LinearIsoElasticPFDamage::LinearIsoElasticPFDamage(const InputParameters & parameters) :
    ComputeStressBase(parameters),
    _c(coupledValue("c")),
    _kdamage(getParam<Real>("kdamage")),
    _G0_pos(declareProperty<Real>("G0_pos")),
    _psi(declareProperty<Real>("psi")),
    _dstress_dc(declarePropertyDerivative<RankTwoTensor>(_base_name + "stress", getVar("c", 0)->name())),
    _dG0_pos_dstrain(declareProperty<RankTwoTensor>("dG0_pos_dstrain")),
    _etens(LIBMESH_DIM),
    _epos(LIBMESH_DIM),
    _eigval(LIBMESH_DIM),
    _elasticity_tensor_name(_base_name + "elasticity_tensor"),
    _elasticity_tensor(getMaterialPropertyByName<RankFourTensor>(_elasticity_tensor_name))
{
}

void LinearIsoElasticPFDamage::computeQpStress()
{
  updateVar();
  updateJacobian();
}

void
LinearIsoElasticPFDamage::updateVar()
{
  RankTwoTensor stress0pos, stress0neg, stress0;
  //Isotropic elasticity is assumed
  Real lambda = _elasticity_tensor[_qp](0,0,1,1);
  Real mu = _elasticity_tensor[_qp](0,1,0,1);
  Real c = _c[_qp];
  Real xfac = Utility::pow<2>(1.0-c) + _kdamage;

  _mechanical_strain[_qp].symmetricEigenvaluesEigenvectors(_eigval, _eigvec);

  //Tensors of outerproduct of eigen vectors
  for (unsigned int i = 0; i < LIBMESH_DIM; ++i)
    for (unsigned int j = 0; j < LIBMESH_DIM; ++j)
      for (unsigned int k = 0; k < LIBMESH_DIM; ++k)
        _etens[i](j,k) = _eigvec(j,i) * _eigvec(k,i);

  Real etr = 0.0;
  for (unsigned int i = 0; i < LIBMESH_DIM; ++i)
    etr += _eigval[i];

  Real etrpos = (std::abs(etr) + etr) / 2.0;
  Real etrneg = (std::abs(etr) - etr) / 2.0;

  for (unsigned int i = 0; i < LIBMESH_DIM; ++i)
  {
    stress0pos += _etens[i] * (lambda * etrpos + 2.0 * mu * (std::abs(_eigval[i]) + _eigval[i]) / 2.0);
    stress0neg += _etens[i] * (lambda * etrneg + 2.0 * mu * (std::abs(_eigval[i]) - _eigval[i]) / 2.0);
  }

  //Damage associated with positive component of stress
  _stress[_qp] = stress0pos * xfac - stress0neg;

  //Calculation of strain energy density
  _psi[_qp] = 0.5*(_stress[_qp].doubleContraction(_mechanical_strain[_qp]));

  for (unsigned int i = 0; i < LIBMESH_DIM; ++i)
    _epos[i] = (std::abs(_eigval[i]) + _eigval[i]) / 2.0;

  Real val = 0.0;
  for (unsigned int i = 0; i < LIBMESH_DIM; ++i)
    val += Utility::pow<2>(_epos[i]);
  val *= mu;

  //Energy with positive principal strains
  _G0_pos[_qp] = lambda * Utility::pow<2>(etrpos) / 2.0 + val;
  //Used in PFFracBulkRate Jacobian
  _dG0_pos_dstrain[_qp] = stress0pos;
  //Used in StressDivergencePFFracTensors Jacobian
  _dstress_dc[_qp] = -stress0pos * (2.0 * (1.0 - c));
}

void
LinearIsoElasticPFDamage::updateJacobian()
{
  _Jacobian_mult[_qp] = _elasticity_tensor[_qp];
}
