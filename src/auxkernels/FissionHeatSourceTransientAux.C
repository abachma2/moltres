#include "FissionHeatSourceTransientAux.h"

registerMooseObject("MoltresApp", FissionHeatSourceTransientAux);

InputParameters
FissionHeatSourceTransientAux::validParams()
{
  InputParameters params = AuxKernel::validParams();
  params.addRequiredParam<unsigned int>("num_groups", "The total numer of energy groups");
  params.addRequiredCoupledVar("group_fluxes",
                               "All the variables that hold the group fluxes. "
                               "These MUST be listed by decreasing "
                               "energy/increasing group number.");
  params.addParam<Real>("nt_scale", 1, "Scaling of the neutron fluxes to aid convergence.");
  return params;
}

FissionHeatSourceTransientAux::FissionHeatSourceTransientAux(const InputParameters & parameters)
  : AuxKernel(parameters),
    _fissxs(getMaterialProperty<std::vector<Real>>("fissxs")),
    _fisse(getMaterialProperty<std::vector<Real>>("fisse")),
    _num_groups(getParam<unsigned int>("num_groups")),
    _nt_scale(getParam<Real>("nt_scale"))
{
  auto n = coupledComponents("group_fluxes");
  if (!(n == _num_groups))
  {
    mooseError("The number of coupled variables doesn't match the number of groups.");
  }
  _group_fluxes.resize(n);
  for (decltype(n) i = 0; i < n; ++i)
    _group_fluxes[i] = &coupledValue("group_fluxes", i);
}

Real
FissionHeatSourceTransientAux::computeValue()
{
  Real r = 0;
  for (unsigned int i = 0; i < _num_groups; ++i)
    r += _fisse[_qp][i] * _fissxs[_qp][i] * (*_group_fluxes[i])[_qp] * _nt_scale;

  return r;
}
