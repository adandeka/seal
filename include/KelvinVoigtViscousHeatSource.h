#ifndef KELVINVOIGTVISCOUSHEATSOURCE_H
#define KELVINVOIGTVISCOUSHEATSOURCE_H

#include "HeatSource.h"
#include "RankTwoTensor.h"

// Forward Declarations
class KelvinVoigtViscousHeatSource;

template <>
InputParameters validParams<KelvinVoigtViscousHeatSource>();

/**
 * This kernel calculates the heat source term corresponding to viscous damping
 */
class KelvinVoigtViscousHeatSource : public HeatSource
{
public:
  KelvinVoigtViscousHeatSource(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

private:
  std::string _base_name;
  const MaterialProperty<RankTwoTensor> & _stress;
  const MaterialProperty<Real> & _heat_source_rate;

  /// Coupled displacement variables
  const unsigned int _ndisp;
  std::vector<unsigned int> _disp_var;
};

#endif // KELVINVOIGTVISCOUSHEATSOURCE_H
