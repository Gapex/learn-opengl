#pragma once

#include "Model.h"
#include "Mesh.h"

class MemoryModel : public Model {
  public:
    MemoryModel();
    void addMesh(Mesh &&mesh);
    ~MemoryModel() override;
};
