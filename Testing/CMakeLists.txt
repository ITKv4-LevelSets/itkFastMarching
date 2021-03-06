include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

#Set output path
SET(TESTING_OUTPUT_PATH "${FMM_BINARY_DIR}/Testing/Temporary")
MAKE_DIRECTORY(${TESTING_OUTPUT_PATH})

add_executable( itkFastMarchingBaseTest
  itkFastMarchingBaseTest.cxx
)
target_link_libraries( itkFastMarchingBaseTest ITKCommon ITKNumerics )

add_test( itkImageFastMarchingBaseTest
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/itkFastMarchingBaseTest 0
)

add_test( itkMeshFastMarchingBaseTest
 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/itkFastMarchingBaseTest 1
)

add_executable( itkFastMarchingImageFilterBaseTest
  itkFastMarchingImageFilterBaseTest.cxx
)
target_link_libraries( itkFastMarchingImageFilterBaseTest ITKCommon ITKNumerics )

add_test( itkFastMarchingImageFilterBaseTest
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/itkFastMarchingImageFilterBaseTest
)


add_executable( itkFastMarchingStoppingCriterionBaseTest
  itkFastMarchingStoppingCriterionBaseTest.cxx
  ${BASE_SOURCE}
)
target_link_libraries( itkFastMarchingStoppingCriterionBaseTest ITKCommon ITKNumerics )


add_executable( itkFastMarchingThresholdStoppingCriterionTest
  itkFastMarchingThresholdStoppingCriterionTest.cxx
  ${BASE_SOURCE}
)
target_link_libraries( itkFastMarchingThresholdStoppingCriterionTest ITKCommon ITKNumerics )

add_executable( itkFastMarchingImageFilterRealTest1
  itkFastMarchingImageFilterRealTest1.cxx
  ${BASE_SOURCE}
)
target_link_libraries( itkFastMarchingImageFilterRealTest1 ITKCommon ITKNumerics )


add_test( itkFastMarchingImageFilterRealTest1
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/itkFastMarchingImageFilterRealTest1 )


add_executable( itkFastMarchingImageFilterRealTest2
  itkFastMarchingImageFilterRealTest2.cxx
  ${BASE_SOURCE}
)
target_link_libraries( itkFastMarchingImageFilterRealTest2 ITKCommon ITKNumerics )

add_test( itkFastMarchingImageFilterRealTest2
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/itkFastMarchingImageFilterRealTest2 )


add_executable( itkFastMarchingUpwindGradientBaseTest
  itkFastMarchingUpwindGradientBaseTest.cxx
  ${BASE_SOURCE}
)

target_link_libraries( itkFastMarchingUpwindGradientBaseTest ITKCommon ITKNumerics )

add_test( itkFastMarchingUpwindGradientBaseTest
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/itkFastMarchingUpwindGradientBaseTest )

add_executable( itkFastMarchingExtensionImageFilterTest
  itkFastMarchingExtensionImageFilterTest.cxx
  ${BASE_SOURCE}
)
target_link_libraries( itkFastMarchingExtensionImageFilterTest ITKCommon ITKNumerics )

add_test( itkFastMarchingExtensionImageFilterTest
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/itkFastMarchingExtensionImageFilterTest )

# --------------------------------------------------------------------------
# Topology constrained front propagation
# --------------------------------------------------------------------------

# non-templated class -- this should be stored in a library and linked in...
ADD_EXECUTABLE( itkFastMarchingImageTopologicalTest
  itkFastMarchingImageTopologicalTest.cxx
  ${BASE_SOURCE}
)
TARGET_LINK_LIBRARIES( itkFastMarchingImageTopologicalTest ITKCommon ITKNumerics ITKIO )

## 2-D

ADD_TEST( TEST_Brain2D_singleSeed_NoTopo
#  --compare ${DATA_DIRECTORY}/test_Brain2D_singleSeed_NoTopo.nii.gz
#            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_Brain2D_singleSeed_NoTopo.nii.gz
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/itkFastMarchingImageTopologicalTest
    2
    ${DATA_DIRECTORY}/BrainProtonDensitySlice_speed.nii.gz
    ${TESTING_OUTPUT_PATH}/test_Brain2D_singleSeed_NoTopo.nii.gz
    ${DATA_DIRECTORY}/BrainProtonDensitySlice_singleSeed.nii.gz
    150 0
)

ADD_TEST( TEST_Brain2D_singleSeed_StrictTopo
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/itkFastMarchingImageTopologicalTest 2
  ${DATA_DIRECTORY}/BrainProtonDensitySlice_speed.nii.gz
  ${TESTING_OUTPUT_PATH}/test_Brain2D_singleSeed_StrictTopo.nii.gz
  ${DATA_DIRECTORY}/BrainProtonDensitySlice_singleSeed.nii.gz
  150 1 )

ADD_TEST( TEST_Brain2D_singleSeed_NoHandlesTopo
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/itkFastMarchingImageTopologicalTest 2
  ${DATA_DIRECTORY}/BrainProtonDensitySlice_speed.nii.gz
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_Brain2D_singleSeed_NoHandlesTopo.nii.gz
  ${DATA_DIRECTORY}/BrainProtonDensitySlice_singleSeed.nii.gz
  150 2 )

ADD_TEST( TEST_Brain2D_multipleSeeds_NoTopo
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/itkFastMarchingImageTopologicalTest 2
  ${DATA_DIRECTORY}/BrainProtonDensitySlice_speed.nii.gz
  ${TESTING_OUTPUT_PATH}/test_Brain2D_multipleSeeds_NoTopo.nii.gz
  ${DATA_DIRECTORY}/BrainProtonDensitySlice_multipleSeeds.nii.gz
  150 0 )

ADD_TEST( TEST_Brain2D_multipleSeeds_StrictTopo
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/itkFastMarchingImageTopologicalTest 2
  ${DATA_DIRECTORY}/BrainProtonDensitySlice_speed.nii.gz
  ${TESTING_OUTPUT_PATH}/test_Brain2D_multipleSeeds_StrictTopo.nii.gz
  ${DATA_DIRECTORY}/BrainProtonDensitySlice_multipleSeeds.nii.gz
  150 1 )

ADD_TEST( TEST_Brain2D_multipleSeeds_NoHandlesTopo
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/itkFastMarchingImageTopologicalTest 2
  ${DATA_DIRECTORY}/BrainProtonDensitySlice_speed.nii.gz
  ${TESTING_OUTPUT_PATH}/test_Brain2D_multipleSeeds_NoHandlesTopo.nii.gz
  ${DATA_DIRECTORY}/BrainProtonDensitySlice_multipleSeeds.nii.gz
  150 2 )

## 3-D

ADD_TEST( TEST_torus_multipleSeeds_NoTopo
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/itkFastMarchingImageTopologicalTest 3
  ${DATA_DIRECTORY}/torus.nii.gz
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_torus_multipleSeeds_NoTopo.nii.gz
  ${DATA_DIRECTORY}/torus_multipleSeeds.nii.gz
  150 0 )

ADD_TEST( TEST_torus_multipleSeeds_StrictTopo
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/itkFastMarchingImageTopologicalTest 3
  ${DATA_DIRECTORY}/torus.nii.gz
  ${TESTING_OUTPUT_PATH}/test_torus_multipleSeeds_StrictTopo.nii.gz
  ${DATA_DIRECTORY}/torus_multipleSeeds.nii.gz
  150 1 )

ADD_TEST( TEST_torus_multipleSeeds_NoHandlesTopo
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/itkFastMarchingImageTopologicalTest 3
  ${DATA_DIRECTORY}/torus.nii.gz
  ${TESTING_OUTPUT_PATH}/test_torus_multipleSeeds_NoHandlesTopo.nii.gz
  ${DATA_DIRECTORY}/torus_multipleSeeds.nii.gz
  150 2 )

ADD_TEST( TEST_wm_multipleSeeds_NoTopo
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/itkFastMarchingImageTopologicalTest 3
  ${DATA_DIRECTORY}/wm.nii.gz
  ${TESTING_OUTPUT_PATH}/test_wm_multipleSeeds_NoTopo.nii.gz
  ${DATA_DIRECTORY}/wm_multipleSeeds.nii.gz
  150 0 )

ADD_TEST( TEST_wm_multipleSeeds_StrictTopo
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/itkFastMarchingImageTopologicalTest 3
  ${DATA_DIRECTORY}/wm.nii.gz
 ${TESTING_OUTPUT_PATH}/test_wm_multipleSeeds_StrictTopo.nii.gz
  ${DATA_DIRECTORY}/wm_multipleSeeds.nii.gz
  150 1 )

ADD_TEST( TEST_wm_multipleSeeds_NoHandlesTopo
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/itkFastMarchingImageTopologicalTest 3
  ${DATA_DIRECTORY}/wm.nii.gz
  ${TESTING_OUTPUT_PATH}/test_wm_multipleSeeds_NoHandlesTopo.nii.gz
  ${DATA_DIRECTORY}/wm_multipleSeeds.nii.gz
  150 2 )

#Set the test drivers
SET(SOURCE_TEST_DRIVER ${EXECUTABLE_OUTPUT_PATH}/MinimalPathTestDriver)

#Set source test files
SET(Source_Test_Files
  MinimalPathTests.cxx
)

include_directories( ${FMM_SOURCE_DIR}/Code )

# Main library
ADD_EXECUTABLE(MinimalPathMain main.cxx ${BASE_SOURCE})
TARGET_LINK_LIBRARIES(MinimalPathMain ITKCommon ITKIO ITKBasicFilters ITKNumerics )

#Add source test driver exe
ADD_EXECUTABLE( MinimalPathTestDriver MinimalPathTestDriver.cxx ${Source_Test_Files} ${BASE_SOURCE} )
TARGET_LINK_LIBRARIES( MinimalPathTestDriver ITKCommon ITKIO ITKBasicFilters ITKNumerics )

#Add tests for the source images
#Noise-01
ADD_TEST(
    Noise_01_NeighborhoodIterate
    ${SOURCE_TEST_DRIVER}
    Test_SpeedToPath_IterateNeighborhood_2D
    ${TESTING_OUTPUT_PATH}/Noise-01-IterateNeighborhood.png
    ${DATA_DIRECTORY}/Noise-01-Speed.mhd
    ${DATA_DIRECTORY}/Noise-01.path
    1.0
    1.0
)
ADD_TEST(
    Noise_01_GradientDescent
    ${SOURCE_TEST_DRIVER}
    Test_SpeedToPath_GradientDescent_2D
    ${TESTING_OUTPUT_PATH}/Noise-01-GradientDescent.png
    ${DATA_DIRECTORY}/Noise-01-Speed.mhd
    ${DATA_DIRECTORY}/Noise-01.path
    1.0
    1000
)
ADD_TEST(
    Noise_01_RegularStepGradientDescent
    ${SOURCE_TEST_DRIVER}
    Test_SpeedToPath_RegularStepGradientDescent_2D
    ${TESTING_OUTPUT_PATH}/Noise-01-RegularStepGradientDescent.png
    ${DATA_DIRECTORY}/Noise-01-Speed.mhd
    ${DATA_DIRECTORY}/Noise-01.path
    1.0
    1000
    1.0
    0.5
)
#Synthetic-01
ADD_TEST(
    Synthetic_01_NeighborhoodIterate
    ${SOURCE_TEST_DRIVER}
    Test_SpeedToPath_IterateNeighborhood_2D
    ${TESTING_OUTPUT_PATH}/Synthetic-01-IterateNeighborhood.png
    ${DATA_DIRECTORY}/Synthetic-01-Speed.mhd
    ${DATA_DIRECTORY}/Synthetic-01.path
    1.0
    1.0
)
ADD_TEST(
    Synthetic_01_GradientDescent
    ${SOURCE_TEST_DRIVER}
    Test_SpeedToPath_GradientDescent_2D
    ${TESTING_OUTPUT_PATH}/Synthetic-01-GradientDescent.png
    ${DATA_DIRECTORY}/Synthetic-01-Speed.mhd
    ${DATA_DIRECTORY}/Synthetic-01.path
    1.0
    1000
)
ADD_TEST(
    Synthetic_01_RegularStepGradientDescent
    ${SOURCE_TEST_DRIVER}
    Test_SpeedToPath_RegularStepGradientDescent_2D
    ${TESTING_OUTPUT_PATH}/Synthetic-01-RegularStepGradientDescent.png
    ${DATA_DIRECTORY}/Synthetic-01-Speed.mhd
    ${DATA_DIRECTORY}/Synthetic-01.path
    1.0
    1000
    1.0
    0.5
)
#Synthetic-02
ADD_TEST(
    Synthetic_02_NeighborhoodIterate
    ${SOURCE_TEST_DRIVER}
    Test_SpeedToPath_IterateNeighborhood_2D
    ${TESTING_OUTPUT_PATH}/Synthetic-02-IterateNeighborhood.png
    ${DATA_DIRECTORY}/Synthetic-02-Speed.mhd
    ${DATA_DIRECTORY}/Synthetic-02.path
    1.0
    1.0
)
ADD_TEST(
    Synthetic_02_GradientDescent
    ${SOURCE_TEST_DRIVER}
    Test_SpeedToPath_GradientDescent_2D
    ${TESTING_OUTPUT_PATH}/Synthetic-02-GradientDescent.png
    ${DATA_DIRECTORY}/Synthetic-02-Speed.mhd
    ${DATA_DIRECTORY}/Synthetic-02.path
    1.0
    3000
)
ADD_TEST(
    Synthetic_02_RegularStepGradientDescent
    ${SOURCE_TEST_DRIVER}
    Test_SpeedToPath_RegularStepGradientDescent_2D
    ${TESTING_OUTPUT_PATH}/Synthetic-02-RegularStepGradientDescent.png
    ${DATA_DIRECTORY}/Synthetic-02-Speed.mhd
    ${DATA_DIRECTORY}/Synthetic-02.path
    1.0
    3000
    1.0
    0.5
)
#Synthetic-03
ADD_TEST(
    Synthetic_03_NeighborhoodIterate
    ${SOURCE_TEST_DRIVER}
    Test_SpeedToPath_IterateNeighborhood_2D
    ${TESTING_OUTPUT_PATH}/Synthetic-03-IterateNeighborhood.png
    ${DATA_DIRECTORY}/Synthetic-03-Speed.mhd
    ${DATA_DIRECTORY}/Synthetic-03.path
    1.0
    1.0
)
#Synthetic-04
ADD_TEST(
    Synthetic_04_NeighborhoodIterate
    ${SOURCE_TEST_DRIVER}
    Test_SpeedToPath_IterateNeighborhood_3D
    ${TESTING_OUTPUT_PATH}/Synthetic-04-IterateNeighborhood.mhd
    ${DATA_DIRECTORY}/Synthetic-04-Speed.mhd
    ${DATA_DIRECTORY}/Synthetic-04.path
    1.0
    1.0
)
ADD_TEST(
    Synthetic_04_GradientDescent
    ${SOURCE_TEST_DRIVER}
    Test_SpeedToPath_GradientDescent_3D
    ${TESTING_OUTPUT_PATH}/Synthetic-04-GradientDescent.mhd
    ${DATA_DIRECTORY}/Synthetic-04-Speed.mhd
    ${DATA_DIRECTORY}/Synthetic-04.path
    1.0
    3000
)
ADD_TEST(
    Synthetic_04_RegularStepGradientDescent
    ${SOURCE_TEST_DRIVER}
    Test_SpeedToPath_RegularStepGradientDescent_3D
    ${TESTING_OUTPUT_PATH}/Synthetic-04-RegularStepGradientDescent.mhd
    ${DATA_DIRECTORY}/Synthetic-04-Speed.mhd
    ${DATA_DIRECTORY}/Synthetic-04.path
    1.0
    3000
    1.0
    0.5
)
#Real-DSA-01-01
ADD_TEST(
    Real_DSA_01_01_NeighborhoodIterate
    ${SOURCE_TEST_DRIVER}
    Test_SpeedToPath_IterateNeighborhood_2D
    ${TESTING_OUTPUT_PATH}/Real-DSA-01-01-IterateNeighborhood.png
    ${DATA_DIRECTORY}/Real-DSA-01-Speed-01.mhd
    ${DATA_DIRECTORY}/Real-DSA-01.path
    1.0
    0.05
)
ADD_TEST(
    Real_DSA_01_01_RegularStepGradientDescent
    ${SOURCE_TEST_DRIVER}
    Test_SpeedToPath_RegularStepGradientDescent_2D
    ${TESTING_OUTPUT_PATH}/Real-DSA-01-01-RegularStepGradientDescent.png
    ${DATA_DIRECTORY}/Real-DSA-01-Speed-01.mhd
    ${DATA_DIRECTORY}/Real-DSA-01.path
    3.0
    4000
    0.25
    0.5
)
#Real-DSA-01-02
ADD_TEST(
    Real_DSA_01_02_NeighborhoodIterate
    ${SOURCE_TEST_DRIVER}
    Test_SpeedToPath_IterateNeighborhood_2D
    ${TESTING_OUTPUT_PATH}/Real-DSA-01-02-IterateNeighborhood.png
    ${DATA_DIRECTORY}/Real-DSA-01-Speed-02.mhd
    ${DATA_DIRECTORY}/Real-DSA-01.path
    1.0
    0.05
)
ADD_TEST(
    Real_DSA_01_02_RegularStepGradientDescent
    ${SOURCE_TEST_DRIVER}
    Test_SpeedToPath_RegularStepGradientDescent_2D
    ${TESTING_OUTPUT_PATH}/Real-DSA-01-02-RegularStepGradientDescent.png
    ${DATA_DIRECTORY}/Real-DSA-01-Speed-02.mhd
    ${DATA_DIRECTORY}/Real-DSA-01.path
    3.0
    4000
    0.25
    0.5
)

