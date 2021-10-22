# CMake file for DISTRHO Plugins #
# ------------------------------ #

dpf_add_plugin(d_stutter
  TARGETS ladspa lv2 vst2
  FILES_DSP
      stutter.cpp)

target_include_directories(
  d_stutter PUBLIC ".")
