local ffi = require 'ffi'
local torch = require 'torch'
local utils = require 'pcl.utils'
local pcl = require 'pcl.PointTypes'

local PCA = torch.class('pcl.PCA', pcl)

local func_by_type = {}

function init()

  local PCA_method_names = {
    'new',
    'clone',
    'delete',
    'set_inputCloud',
    'get_mean',
    'get_eigenVectors',
    'get_eigenValues',
    'get_coefficients',
    'project_cloud',
    'reconstruct_cloud'
  }
  local base_name = 'pcl_PCL_XYZRGBA_'
  
  local supported_types = {}
  supported_types[pcl.PointXYZ] = 'XYZ'
  supported_types[pcl.PointXYZI] = 'XYZI'
  supported_types[pcl.PointXYZRGBA] = 'XYZRGBA'
  
  for k,v in pairs(supported_types) do
    func_by_type[k] = utils.create_typed_methods("pcl_PCA_TYPE_KEY_", PCA_method_names, v)
  end
end

init()

function PCA:__init(pointType, basis_only)
  local cloud
  if torch.isTypeOf(pointType, pcl.PointCloud) then
    cloud = pointType
    pointType = cloud.pointType
  end
  
  pointType = pointType or pcl.PointXYZ
  basis_only = basis_only or false
  self.pointType = pointType
  self.f = func_by_type[self.pointType]
  
  if type(basis_only) == 'cdata' then   -- used by clone
    self.o = basis_only
  else
    self.o = self.f.new(basis_only)
  end

  if cloud then
    self:set_inputCloud(cloud)
  end
end

function PCA:cdata()
  return self.o
end

function PCA:clone()
  local clone = self.f.clone(self.o)
  return PCA.new(self.pointType, clone)
end

function PCA:set_inputCloud(cloud)
  self.f.set_inputCloud(self.o, cloud:cdata())
end

function PCA:get_mean(t)
  t = t or torch.FloatTensor()
  self.f.get_mean(self.o, t:cdata())
  return t;
end

function PCA:get_eigenVectors(t)
  t = t or torch.FloatTensor()
  self.f.get_eigenVectors(self.o, t:cdata())
  return t;
end

function PCA:get_eigenValues(t)
  t = t or torch.FloatTensor()
  self.f.get_eigenValues(self.o, t:cdata())
  return t;
end

function PCA:get_coefficients(t)
  t = t or torch.FloatTensor()
  self.f.get_coefficients(self.o, t:cdata())
  return t;
end

function PCA:project(input, output)
  ouput = output or input
  self.f.project_cloud(self.c, input:cadat(), output:cdata())
end

function PCA:reconstruct(input, output)
  output = output or input
  self.f.reconstruct_cloud(self.c, input:cadat(), output:cdata())
end
