-- premake5.lua
workspace "IuvoParticles"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "IuvoParticles"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "WalnutExternal.lua"
include "IuvoParticlesApp"