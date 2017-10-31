##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=vzray
ConfigurationName      :=Debug
WorkspacePath          :=C:/WorkSpace/vzray
ProjectPath            :=C:/WorkSpace/vzray
IntermediateDirectory  :=./obj
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=aLFoNSo
Date                   :=28/10/2017
CodeLitePath           :="C:/Archivos de programa/CodeLite"
LinkerName             :=C:/TDM-GCC-32/bin/g++.exe
SharedObjectLinkerName :=C:/TDM-GCC-32/bin/g++.exe -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=./bin/$(ConfigurationName)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="vzray.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/TDM-GCC-32/bin/windres.exe
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)./src 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/TDM-GCC-32/bin/ar.exe rcu
CXX      := C:/TDM-GCC-32/bin/g++.exe
CC       := C:/TDM-GCC-32/bin/gcc.exe
CXXFLAGS :=  -g -O0 -std=c++14 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := C:/TDM-GCC-32/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Archivos de programa\CodeLite
WXWIN:=C:/wx302
Objects0=$(IntermediateDirectory)/src_image.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_log.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_onb.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_point.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_ray.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_rgb.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_statistics.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_test.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_vec2.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_vec3.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/src_vzray.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_aabb_aabb.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_bvh_bvh.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_camera_pinhole.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_camera_thinlens.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_material_dielectricmaterial.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_material_diffusematerial.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_material_lightmaterial.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_material_specularmaterial.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_matrix_matrix.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/src_matrix_transform.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_parser_parser.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_randomc_randomc.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_renderer_directrenderer.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_renderer_pathrenderer.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_renderer_whittedrenderer.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_scene_scene.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_shapes_box.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_shapes_cylinder.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_shapes_disc.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/src_shapes_isecaux.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_shapes_mesh.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_shapes_meshtriangle.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_shapes_parallelogram.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_shapes_plane.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_shapes_sphere.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_shapes_triangle.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@$(MakeDirCommand) "./obj"


$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./obj"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/src_image.cpp$(ObjectSuffix): src/image.cpp $(IntermediateDirectory)/src_image.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/WorkSpace/vzray/src/image.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_image.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_image.cpp$(DependSuffix): src/image.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_image.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_image.cpp$(DependSuffix) -MM src/image.cpp

$(IntermediateDirectory)/src_image.cpp$(PreprocessSuffix): src/image.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_image.cpp$(PreprocessSuffix) src/image.cpp

$(IntermediateDirectory)/src_log.cpp$(ObjectSuffix): src/log.cpp $(IntermediateDirectory)/src_log.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/WorkSpace/vzray/src/log.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_log.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_log.cpp$(DependSuffix): src/log.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_log.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_log.cpp$(DependSuffix) -MM src/log.cpp

$(IntermediateDirectory)/src_log.cpp$(PreprocessSuffix): src/log.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_log.cpp$(PreprocessSuffix) src/log.cpp

$(IntermediateDirectory)/src_onb.cpp$(ObjectSuffix): src/onb.cpp $(IntermediateDirectory)/src_onb.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/WorkSpace/vzray/src/onb.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_onb.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_onb.cpp$(DependSuffix): src/onb.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_onb.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_onb.cpp$(DependSuffix) -MM src/onb.cpp

$(IntermediateDirectory)/src_onb.cpp$(PreprocessSuffix): src/onb.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_onb.cpp$(PreprocessSuffix) src/onb.cpp

$(IntermediateDirectory)/src_point.cpp$(ObjectSuffix): src/point.cpp $(IntermediateDirectory)/src_point.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/WorkSpace/vzray/src/point.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_point.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_point.cpp$(DependSuffix): src/point.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_point.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_point.cpp$(DependSuffix) -MM src/point.cpp

$(IntermediateDirectory)/src_point.cpp$(PreprocessSuffix): src/point.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_point.cpp$(PreprocessSuffix) src/point.cpp

$(IntermediateDirectory)/src_ray.cpp$(ObjectSuffix): src/ray.cpp $(IntermediateDirectory)/src_ray.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/WorkSpace/vzray/src/ray.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_ray.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_ray.cpp$(DependSuffix): src/ray.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_ray.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_ray.cpp$(DependSuffix) -MM src/ray.cpp

$(IntermediateDirectory)/src_ray.cpp$(PreprocessSuffix): src/ray.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_ray.cpp$(PreprocessSuffix) src/ray.cpp

$(IntermediateDirectory)/src_rgb.cpp$(ObjectSuffix): src/rgb.cpp $(IntermediateDirectory)/src_rgb.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/WorkSpace/vzray/src/rgb.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_rgb.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_rgb.cpp$(DependSuffix): src/rgb.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_rgb.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_rgb.cpp$(DependSuffix) -MM src/rgb.cpp

$(IntermediateDirectory)/src_rgb.cpp$(PreprocessSuffix): src/rgb.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_rgb.cpp$(PreprocessSuffix) src/rgb.cpp

$(IntermediateDirectory)/src_statistics.cpp$(ObjectSuffix): src/statistics.cpp $(IntermediateDirectory)/src_statistics.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/WorkSpace/vzray/src/statistics.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_statistics.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_statistics.cpp$(DependSuffix): src/statistics.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_statistics.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_statistics.cpp$(DependSuffix) -MM src/statistics.cpp

$(IntermediateDirectory)/src_statistics.cpp$(PreprocessSuffix): src/statistics.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_statistics.cpp$(PreprocessSuffix) src/statistics.cpp

$(IntermediateDirectory)/src_test.cpp$(ObjectSuffix): src/test.cpp $(IntermediateDirectory)/src_test.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/WorkSpace/vzray/src/test.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_test.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_test.cpp$(DependSuffix): src/test.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_test.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_test.cpp$(DependSuffix) -MM src/test.cpp

$(IntermediateDirectory)/src_test.cpp$(PreprocessSuffix): src/test.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_test.cpp$(PreprocessSuffix) src/test.cpp

$(IntermediateDirectory)/src_vec2.cpp$(ObjectSuffix): src/vec2.cpp $(IntermediateDirectory)/src_vec2.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/WorkSpace/vzray/src/vec2.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_vec2.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_vec2.cpp$(DependSuffix): src/vec2.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_vec2.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_vec2.cpp$(DependSuffix) -MM src/vec2.cpp

$(IntermediateDirectory)/src_vec2.cpp$(PreprocessSuffix): src/vec2.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_vec2.cpp$(PreprocessSuffix) src/vec2.cpp

$(IntermediateDirectory)/src_vec3.cpp$(ObjectSuffix): src/vec3.cpp $(IntermediateDirectory)/src_vec3.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/WorkSpace/vzray/src/vec3.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_vec3.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_vec3.cpp$(DependSuffix): src/vec3.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_vec3.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_vec3.cpp$(DependSuffix) -MM src/vec3.cpp

$(IntermediateDirectory)/src_vec3.cpp$(PreprocessSuffix): src/vec3.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_vec3.cpp$(PreprocessSuffix) src/vec3.cpp

$(IntermediateDirectory)/src_vzray.cpp$(ObjectSuffix): src/vzray.cpp $(IntermediateDirectory)/src_vzray.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/WorkSpace/vzray/src/vzray.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_vzray.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_vzray.cpp$(DependSuffix): src/vzray.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_vzray.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_vzray.cpp$(DependSuffix) -MM src/vzray.cpp

$(IntermediateDirectory)/src_vzray.cpp$(PreprocessSuffix): src/vzray.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_vzray.cpp$(PreprocessSuffix) src/vzray.cpp

$(IntermediateDirectory)/src_aabb_aabb.cpp$(ObjectSuffix): src/aabb/aabb.cpp $(IntermediateDirectory)/src_aabb_aabb.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/WorkSpace/vzray/src/aabb/aabb.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_aabb_aabb.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_aabb_aabb.cpp$(DependSuffix): src/aabb/aabb.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_aabb_aabb.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_aabb_aabb.cpp$(DependSuffix) -MM src/aabb/aabb.cpp

$(IntermediateDirectory)/src_aabb_aabb.cpp$(PreprocessSuffix): src/aabb/aabb.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_aabb_aabb.cpp$(PreprocessSuffix) src/aabb/aabb.cpp

$(IntermediateDirectory)/src_bvh_bvh.cpp$(ObjectSuffix): src/bvh/bvh.cpp $(IntermediateDirectory)/src_bvh_bvh.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/WorkSpace/vzray/src/bvh/bvh.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_bvh_bvh.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_bvh_bvh.cpp$(DependSuffix): src/bvh/bvh.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_bvh_bvh.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_bvh_bvh.cpp$(DependSuffix) -MM src/bvh/bvh.cpp

$(IntermediateDirectory)/src_bvh_bvh.cpp$(PreprocessSuffix): src/bvh/bvh.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_bvh_bvh.cpp$(PreprocessSuffix) src/bvh/bvh.cpp

$(IntermediateDirectory)/src_camera_pinhole.cpp$(ObjectSuffix): src/camera/pinhole.cpp $(IntermediateDirectory)/src_camera_pinhole.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/WorkSpace/vzray/src/camera/pinhole.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_camera_pinhole.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_camera_pinhole.cpp$(DependSuffix): src/camera/pinhole.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_camera_pinhole.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_camera_pinhole.cpp$(DependSuffix) -MM src/camera/pinhole.cpp

$(IntermediateDirectory)/src_camera_pinhole.cpp$(PreprocessSuffix): src/camera/pinhole.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_camera_pinhole.cpp$(PreprocessSuffix) src/camera/pinhole.cpp

$(IntermediateDirectory)/src_camera_thinlens.cpp$(ObjectSuffix): src/camera/thinlens.cpp $(IntermediateDirectory)/src_camera_thinlens.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/WorkSpace/vzray/src/camera/thinlens.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_camera_thinlens.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_camera_thinlens.cpp$(DependSuffix): src/camera/thinlens.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_camera_thinlens.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_camera_thinlens.cpp$(DependSuffix) -MM src/camera/thinlens.cpp

$(IntermediateDirectory)/src_camera_thinlens.cpp$(PreprocessSuffix): src/camera/thinlens.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_camera_thinlens.cpp$(PreprocessSuffix) src/camera/thinlens.cpp

$(IntermediateDirectory)/src_material_dielectricmaterial.cpp$(ObjectSuffix): src/material/dielectricmaterial.cpp $(IntermediateDirectory)/src_material_dielectricmaterial.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/WorkSpace/vzray/src/material/dielectricmaterial.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_material_dielectricmaterial.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_material_dielectricmaterial.cpp$(DependSuffix): src/material/dielectricmaterial.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_material_dielectricmaterial.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_material_dielectricmaterial.cpp$(DependSuffix) -MM src/material/dielectricmaterial.cpp

$(IntermediateDirectory)/src_material_dielectricmaterial.cpp$(PreprocessSuffix): src/material/dielectricmaterial.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_material_dielectricmaterial.cpp$(PreprocessSuffix) src/material/dielectricmaterial.cpp

$(IntermediateDirectory)/src_material_diffusematerial.cpp$(ObjectSuffix): src/material/diffusematerial.cpp $(IntermediateDirectory)/src_material_diffusematerial.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/WorkSpace/vzray/src/material/diffusematerial.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_material_diffusematerial.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_material_diffusematerial.cpp$(DependSuffix): src/material/diffusematerial.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_material_diffusematerial.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_material_diffusematerial.cpp$(DependSuffix) -MM src/material/diffusematerial.cpp

$(IntermediateDirectory)/src_material_diffusematerial.cpp$(PreprocessSuffix): src/material/diffusematerial.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_material_diffusematerial.cpp$(PreprocessSuffix) src/material/diffusematerial.cpp

$(IntermediateDirectory)/src_material_lightmaterial.cpp$(ObjectSuffix): src/material/lightmaterial.cpp $(IntermediateDirectory)/src_material_lightmaterial.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/WorkSpace/vzray/src/material/lightmaterial.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_material_lightmaterial.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_material_lightmaterial.cpp$(DependSuffix): src/material/lightmaterial.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_material_lightmaterial.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_material_lightmaterial.cpp$(DependSuffix) -MM src/material/lightmaterial.cpp

$(IntermediateDirectory)/src_material_lightmaterial.cpp$(PreprocessSuffix): src/material/lightmaterial.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_material_lightmaterial.cpp$(PreprocessSuffix) src/material/lightmaterial.cpp

$(IntermediateDirectory)/src_material_specularmaterial.cpp$(ObjectSuffix): src/material/specularmaterial.cpp $(IntermediateDirectory)/src_material_specularmaterial.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/WorkSpace/vzray/src/material/specularmaterial.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_material_specularmaterial.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_material_specularmaterial.cpp$(DependSuffix): src/material/specularmaterial.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_material_specularmaterial.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_material_specularmaterial.cpp$(DependSuffix) -MM src/material/specularmaterial.cpp

$(IntermediateDirectory)/src_material_specularmaterial.cpp$(PreprocessSuffix): src/material/specularmaterial.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_material_specularmaterial.cpp$(PreprocessSuffix) src/material/specularmaterial.cpp

$(IntermediateDirectory)/src_matrix_matrix.cpp$(ObjectSuffix): src/matrix/matrix.cpp $(IntermediateDirectory)/src_matrix_matrix.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/WorkSpace/vzray/src/matrix/matrix.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_matrix_matrix.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_matrix_matrix.cpp$(DependSuffix): src/matrix/matrix.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_matrix_matrix.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_matrix_matrix.cpp$(DependSuffix) -MM src/matrix/matrix.cpp

$(IntermediateDirectory)/src_matrix_matrix.cpp$(PreprocessSuffix): src/matrix/matrix.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_matrix_matrix.cpp$(PreprocessSuffix) src/matrix/matrix.cpp

$(IntermediateDirectory)/src_matrix_transform.cpp$(ObjectSuffix): src/matrix/transform.cpp $(IntermediateDirectory)/src_matrix_transform.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/WorkSpace/vzray/src/matrix/transform.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_matrix_transform.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_matrix_transform.cpp$(DependSuffix): src/matrix/transform.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_matrix_transform.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_matrix_transform.cpp$(DependSuffix) -MM src/matrix/transform.cpp

$(IntermediateDirectory)/src_matrix_transform.cpp$(PreprocessSuffix): src/matrix/transform.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_matrix_transform.cpp$(PreprocessSuffix) src/matrix/transform.cpp

$(IntermediateDirectory)/src_parser_parser.cpp$(ObjectSuffix): src/parser/parser.cpp $(IntermediateDirectory)/src_parser_parser.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/WorkSpace/vzray/src/parser/parser.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_parser_parser.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_parser_parser.cpp$(DependSuffix): src/parser/parser.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_parser_parser.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_parser_parser.cpp$(DependSuffix) -MM src/parser/parser.cpp

$(IntermediateDirectory)/src_parser_parser.cpp$(PreprocessSuffix): src/parser/parser.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_parser_parser.cpp$(PreprocessSuffix) src/parser/parser.cpp

$(IntermediateDirectory)/src_randomc_randomc.cpp$(ObjectSuffix): src/randomc/randomc.cpp $(IntermediateDirectory)/src_randomc_randomc.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/WorkSpace/vzray/src/randomc/randomc.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_randomc_randomc.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_randomc_randomc.cpp$(DependSuffix): src/randomc/randomc.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_randomc_randomc.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_randomc_randomc.cpp$(DependSuffix) -MM src/randomc/randomc.cpp

$(IntermediateDirectory)/src_randomc_randomc.cpp$(PreprocessSuffix): src/randomc/randomc.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_randomc_randomc.cpp$(PreprocessSuffix) src/randomc/randomc.cpp

$(IntermediateDirectory)/src_renderer_directrenderer.cpp$(ObjectSuffix): src/renderer/directrenderer.cpp $(IntermediateDirectory)/src_renderer_directrenderer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/WorkSpace/vzray/src/renderer/directrenderer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_renderer_directrenderer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_renderer_directrenderer.cpp$(DependSuffix): src/renderer/directrenderer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_renderer_directrenderer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_renderer_directrenderer.cpp$(DependSuffix) -MM src/renderer/directrenderer.cpp

$(IntermediateDirectory)/src_renderer_directrenderer.cpp$(PreprocessSuffix): src/renderer/directrenderer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_renderer_directrenderer.cpp$(PreprocessSuffix) src/renderer/directrenderer.cpp

$(IntermediateDirectory)/src_renderer_pathrenderer.cpp$(ObjectSuffix): src/renderer/pathrenderer.cpp $(IntermediateDirectory)/src_renderer_pathrenderer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/WorkSpace/vzray/src/renderer/pathrenderer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_renderer_pathrenderer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_renderer_pathrenderer.cpp$(DependSuffix): src/renderer/pathrenderer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_renderer_pathrenderer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_renderer_pathrenderer.cpp$(DependSuffix) -MM src/renderer/pathrenderer.cpp

$(IntermediateDirectory)/src_renderer_pathrenderer.cpp$(PreprocessSuffix): src/renderer/pathrenderer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_renderer_pathrenderer.cpp$(PreprocessSuffix) src/renderer/pathrenderer.cpp

$(IntermediateDirectory)/src_renderer_whittedrenderer.cpp$(ObjectSuffix): src/renderer/whittedrenderer.cpp $(IntermediateDirectory)/src_renderer_whittedrenderer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/WorkSpace/vzray/src/renderer/whittedrenderer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_renderer_whittedrenderer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_renderer_whittedrenderer.cpp$(DependSuffix): src/renderer/whittedrenderer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_renderer_whittedrenderer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_renderer_whittedrenderer.cpp$(DependSuffix) -MM src/renderer/whittedrenderer.cpp

$(IntermediateDirectory)/src_renderer_whittedrenderer.cpp$(PreprocessSuffix): src/renderer/whittedrenderer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_renderer_whittedrenderer.cpp$(PreprocessSuffix) src/renderer/whittedrenderer.cpp

$(IntermediateDirectory)/src_scene_scene.cpp$(ObjectSuffix): src/scene/scene.cpp $(IntermediateDirectory)/src_scene_scene.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/WorkSpace/vzray/src/scene/scene.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_scene_scene.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_scene_scene.cpp$(DependSuffix): src/scene/scene.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_scene_scene.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_scene_scene.cpp$(DependSuffix) -MM src/scene/scene.cpp

$(IntermediateDirectory)/src_scene_scene.cpp$(PreprocessSuffix): src/scene/scene.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_scene_scene.cpp$(PreprocessSuffix) src/scene/scene.cpp

$(IntermediateDirectory)/src_shapes_box.cpp$(ObjectSuffix): src/shapes/box.cpp $(IntermediateDirectory)/src_shapes_box.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/WorkSpace/vzray/src/shapes/box.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_shapes_box.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_shapes_box.cpp$(DependSuffix): src/shapes/box.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_shapes_box.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_shapes_box.cpp$(DependSuffix) -MM src/shapes/box.cpp

$(IntermediateDirectory)/src_shapes_box.cpp$(PreprocessSuffix): src/shapes/box.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_shapes_box.cpp$(PreprocessSuffix) src/shapes/box.cpp

$(IntermediateDirectory)/src_shapes_cylinder.cpp$(ObjectSuffix): src/shapes/cylinder.cpp $(IntermediateDirectory)/src_shapes_cylinder.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/WorkSpace/vzray/src/shapes/cylinder.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_shapes_cylinder.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_shapes_cylinder.cpp$(DependSuffix): src/shapes/cylinder.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_shapes_cylinder.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_shapes_cylinder.cpp$(DependSuffix) -MM src/shapes/cylinder.cpp

$(IntermediateDirectory)/src_shapes_cylinder.cpp$(PreprocessSuffix): src/shapes/cylinder.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_shapes_cylinder.cpp$(PreprocessSuffix) src/shapes/cylinder.cpp

$(IntermediateDirectory)/src_shapes_disc.cpp$(ObjectSuffix): src/shapes/disc.cpp $(IntermediateDirectory)/src_shapes_disc.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/WorkSpace/vzray/src/shapes/disc.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_shapes_disc.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_shapes_disc.cpp$(DependSuffix): src/shapes/disc.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_shapes_disc.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_shapes_disc.cpp$(DependSuffix) -MM src/shapes/disc.cpp

$(IntermediateDirectory)/src_shapes_disc.cpp$(PreprocessSuffix): src/shapes/disc.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_shapes_disc.cpp$(PreprocessSuffix) src/shapes/disc.cpp

$(IntermediateDirectory)/src_shapes_isecaux.cpp$(ObjectSuffix): src/shapes/isecaux.cpp $(IntermediateDirectory)/src_shapes_isecaux.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/WorkSpace/vzray/src/shapes/isecaux.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_shapes_isecaux.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_shapes_isecaux.cpp$(DependSuffix): src/shapes/isecaux.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_shapes_isecaux.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_shapes_isecaux.cpp$(DependSuffix) -MM src/shapes/isecaux.cpp

$(IntermediateDirectory)/src_shapes_isecaux.cpp$(PreprocessSuffix): src/shapes/isecaux.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_shapes_isecaux.cpp$(PreprocessSuffix) src/shapes/isecaux.cpp

$(IntermediateDirectory)/src_shapes_mesh.cpp$(ObjectSuffix): src/shapes/mesh.cpp $(IntermediateDirectory)/src_shapes_mesh.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/WorkSpace/vzray/src/shapes/mesh.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_shapes_mesh.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_shapes_mesh.cpp$(DependSuffix): src/shapes/mesh.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_shapes_mesh.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_shapes_mesh.cpp$(DependSuffix) -MM src/shapes/mesh.cpp

$(IntermediateDirectory)/src_shapes_mesh.cpp$(PreprocessSuffix): src/shapes/mesh.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_shapes_mesh.cpp$(PreprocessSuffix) src/shapes/mesh.cpp

$(IntermediateDirectory)/src_shapes_meshtriangle.cpp$(ObjectSuffix): src/shapes/meshtriangle.cpp $(IntermediateDirectory)/src_shapes_meshtriangle.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/WorkSpace/vzray/src/shapes/meshtriangle.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_shapes_meshtriangle.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_shapes_meshtriangle.cpp$(DependSuffix): src/shapes/meshtriangle.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_shapes_meshtriangle.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_shapes_meshtriangle.cpp$(DependSuffix) -MM src/shapes/meshtriangle.cpp

$(IntermediateDirectory)/src_shapes_meshtriangle.cpp$(PreprocessSuffix): src/shapes/meshtriangle.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_shapes_meshtriangle.cpp$(PreprocessSuffix) src/shapes/meshtriangle.cpp

$(IntermediateDirectory)/src_shapes_parallelogram.cpp$(ObjectSuffix): src/shapes/parallelogram.cpp $(IntermediateDirectory)/src_shapes_parallelogram.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/WorkSpace/vzray/src/shapes/parallelogram.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_shapes_parallelogram.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_shapes_parallelogram.cpp$(DependSuffix): src/shapes/parallelogram.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_shapes_parallelogram.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_shapes_parallelogram.cpp$(DependSuffix) -MM src/shapes/parallelogram.cpp

$(IntermediateDirectory)/src_shapes_parallelogram.cpp$(PreprocessSuffix): src/shapes/parallelogram.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_shapes_parallelogram.cpp$(PreprocessSuffix) src/shapes/parallelogram.cpp

$(IntermediateDirectory)/src_shapes_plane.cpp$(ObjectSuffix): src/shapes/plane.cpp $(IntermediateDirectory)/src_shapes_plane.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/WorkSpace/vzray/src/shapes/plane.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_shapes_plane.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_shapes_plane.cpp$(DependSuffix): src/shapes/plane.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_shapes_plane.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_shapes_plane.cpp$(DependSuffix) -MM src/shapes/plane.cpp

$(IntermediateDirectory)/src_shapes_plane.cpp$(PreprocessSuffix): src/shapes/plane.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_shapes_plane.cpp$(PreprocessSuffix) src/shapes/plane.cpp

$(IntermediateDirectory)/src_shapes_sphere.cpp$(ObjectSuffix): src/shapes/sphere.cpp $(IntermediateDirectory)/src_shapes_sphere.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/WorkSpace/vzray/src/shapes/sphere.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_shapes_sphere.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_shapes_sphere.cpp$(DependSuffix): src/shapes/sphere.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_shapes_sphere.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_shapes_sphere.cpp$(DependSuffix) -MM src/shapes/sphere.cpp

$(IntermediateDirectory)/src_shapes_sphere.cpp$(PreprocessSuffix): src/shapes/sphere.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_shapes_sphere.cpp$(PreprocessSuffix) src/shapes/sphere.cpp

$(IntermediateDirectory)/src_shapes_triangle.cpp$(ObjectSuffix): src/shapes/triangle.cpp $(IntermediateDirectory)/src_shapes_triangle.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/WorkSpace/vzray/src/shapes/triangle.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_shapes_triangle.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_shapes_triangle.cpp$(DependSuffix): src/shapes/triangle.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_shapes_triangle.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_shapes_triangle.cpp$(DependSuffix) -MM src/shapes/triangle.cpp

$(IntermediateDirectory)/src_shapes_triangle.cpp$(PreprocessSuffix): src/shapes/triangle.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_shapes_triangle.cpp$(PreprocessSuffix) src/shapes/triangle.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./obj/


