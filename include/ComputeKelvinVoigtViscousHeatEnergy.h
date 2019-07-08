/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef COMPUTEKELVINVOIGTVISCOUSHEATENERGY_H
#define COMPUTEKELVINVOIGTVISCOUSHEATENERGY_H

#include "DerivativeMaterialInterface.h"
#include "Material.h"
#include "RankTwoTensor.h"
#include "RankFourTensor.h"

class ComputeKelvinVoigtViscousHeatEnergy;

template <>
InputParameters validParams<ComputeKelvinVoigtViscousHeatEnergy>();

/**
 * ComputeKelvinVoigtViscousHeatEnergy computes the energy from viscous dissipation
 * when material is represented by single Kelvin Voigt unit
 */

class ComputeKelvinVoigtViscousHeatEnergy : public DerivativeMaterialInterface<Material>
{
public:
  ComputeKelvinVoigtViscousHeatEnergy(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  /// optional parameter that allows multiple mechanics materials to be defined
  std::string _base_name;

  const MaterialProperty<RankTwoTensor> & _stress;
  const MaterialProperty<RankTwoTensor> & _strain_rate;

  MaterialProperty<Real> & _heat_source_rate;
};

#endif // COMPUTEKELVINVOIGTVISCOUSHEATENERGY_H
