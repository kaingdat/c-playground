include ../Makefile.inc

GEN_EXE = us_xfr_cl us_xfr_sv

LINUX_EXE =

EXE = ${GEN_EXE} ${LINUX_EXE}

all : ${EXE}

allgen : ${GEN_EXE}

clean : 
	${RM} ${EXE} *.o

showall :
	@ echo ${EXE}

us_xfr_sv.o us_xfr_cl.o : us_xfr.h

${EXE} : ${TLPI_LIB}		# True as a rough approximation
