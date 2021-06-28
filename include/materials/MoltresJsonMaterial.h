#pragma once

#include "NuclearMaterial.h"
#include "nlohmann/json.h"

class MoltresJsonMaterial : public NuclearMaterial
{
public:
  MoltresJsonMaterial(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  void Construct(nlohmann::json xs_root, std::vector<std::string> xsec_names);
  virtual void computeQpProperties() override;

  std::string _material_key;
};
