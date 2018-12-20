#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/boolLogicParser.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/regReplaceExtension.o \
	${OBJECTDIR}/src/ChannelEntitys/Channel_Entities_PiFace.o \
	${OBJECTDIR}/src/ChannelEntitys/Channel_Entity.o \
	${OBJECTDIR}/src/IOChannels/IO_Channel.o \
	${OBJECTDIR}/src/IOChannels/IO_Channel_Hw.o \
	${OBJECTDIR}/src/IOChannels/IO_Channel_Hw_PiFace.o \
	${OBJECTDIR}/src/IOChannels/IO_Channel_Virtual.o \
	${OBJECTDIR}/src/IOChannels/IO_Channel_Virtual_Memory.o \
	${OBJECTDIR}/src/IOChannels/IO_Channel_Virtual_Pipe.o \
	${OBJECTDIR}/src/IOChannels/IO_Channel_Virtual_Timer.o


# C Compiler Flags
CFLAGS=-j 4

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=/usr/local/lib/libpifacedigitalcpp.a /usr/local/lib/libmcp23s17.a

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mycontroldigital

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mycontroldigital: /usr/local/lib/libpifacedigitalcpp.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mycontroldigital: /usr/local/lib/libmcp23s17.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mycontroldigital: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mycontroldigital ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/boolLogicParser.o: boolLogicParser.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/boolLogicParser.o boolLogicParser.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/regReplaceExtension.o: regReplaceExtension.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/regReplaceExtension.o regReplaceExtension.cpp

${OBJECTDIR}/src/ChannelEntitys/Channel_Entities_PiFace.o: src/ChannelEntitys/Channel_Entities_PiFace.cpp
	${MKDIR} -p ${OBJECTDIR}/src/ChannelEntitys
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ChannelEntitys/Channel_Entities_PiFace.o src/ChannelEntitys/Channel_Entities_PiFace.cpp

${OBJECTDIR}/src/ChannelEntitys/Channel_Entity.o: src/ChannelEntitys/Channel_Entity.cpp
	${MKDIR} -p ${OBJECTDIR}/src/ChannelEntitys
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ChannelEntitys/Channel_Entity.o src/ChannelEntitys/Channel_Entity.cpp

${OBJECTDIR}/src/IOChannels/IO_Channel.o: src/IOChannels/IO_Channel.cpp
	${MKDIR} -p ${OBJECTDIR}/src/IOChannels
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/IOChannels/IO_Channel.o src/IOChannels/IO_Channel.cpp

${OBJECTDIR}/src/IOChannels/IO_Channel_Hw.o: src/IOChannels/IO_Channel_Hw.cpp
	${MKDIR} -p ${OBJECTDIR}/src/IOChannels
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/IOChannels/IO_Channel_Hw.o src/IOChannels/IO_Channel_Hw.cpp

${OBJECTDIR}/src/IOChannels/IO_Channel_Hw_PiFace.o: src/IOChannels/IO_Channel_Hw_PiFace.cpp
	${MKDIR} -p ${OBJECTDIR}/src/IOChannels
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/IOChannels/IO_Channel_Hw_PiFace.o src/IOChannels/IO_Channel_Hw_PiFace.cpp

${OBJECTDIR}/src/IOChannels/IO_Channel_Virtual.o: src/IOChannels/IO_Channel_Virtual.cpp
	${MKDIR} -p ${OBJECTDIR}/src/IOChannels
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/IOChannels/IO_Channel_Virtual.o src/IOChannels/IO_Channel_Virtual.cpp

${OBJECTDIR}/src/IOChannels/IO_Channel_Virtual_Memory.o: src/IOChannels/IO_Channel_Virtual_Memory.cpp
	${MKDIR} -p ${OBJECTDIR}/src/IOChannels
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/IOChannels/IO_Channel_Virtual_Memory.o src/IOChannels/IO_Channel_Virtual_Memory.cpp

${OBJECTDIR}/src/IOChannels/IO_Channel_Virtual_Pipe.o: src/IOChannels/IO_Channel_Virtual_Pipe.cpp
	${MKDIR} -p ${OBJECTDIR}/src/IOChannels
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/IOChannels/IO_Channel_Virtual_Pipe.o src/IOChannels/IO_Channel_Virtual_Pipe.cpp

${OBJECTDIR}/src/IOChannels/IO_Channel_Virtual_Timer.o: src/IOChannels/IO_Channel_Virtual_Timer.cpp
	${MKDIR} -p ${OBJECTDIR}/src/IOChannels
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/IOChannels/IO_Channel_Virtual_Timer.o src/IOChannels/IO_Channel_Virtual_Timer.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
