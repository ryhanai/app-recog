CXXFLAGS = `rtm-config --cflags` -I.
LDFLAGS  = `pkg-config --libs opencv`
#LDFLAGS	= `rospack libs-only-l opencv2`
LDFLAGS += `rtm-config --libs`
SHFLAGS  = -shared

IDLC     = `rtm-config --idlc`
IDLFLAGS = `rtm-config --idlflags` -I`rtm-config --prefix`/include/rtm/idl
WRAPPER  = rtm-skelwrapper
WRAPPER_FLAGS = --include-dir="" --skel-suffix=Skel --stub-suffix=Stub

.SUFFIXES : .so

LSFX = so

TARGET   = AppRecog
TARGETSERVICE   = AppRecog
BINDIR = ./build/bin
OBJDIR = ./build/obj
LIBDIR = ./build/lib
SKEL_OBJ = ImgSkel.o VisionSkel.o
STUB_OBJ = #VisionStub.o ImgStub.o
IMPL_OBJ = ImgSVC_impl.o VisionSVC_impl.o
OBJFILES = $(TARGET).o time.o simpsolib.o simtstlib.o Vision.o $(SKEL_OBJ) $(STUB_OBJ) $(IMPL_OBJ)
OBJS     = $(addprefix $(OBJDIR)/, $(OBJFILES))
#TARGETLIB = $(LIBDIR)/$(TARGET).$(LSFX)
TARGETCOMP = $(BINDIR)/$(TARGET)Comp
#CXXFLAGS += -march=native
CXXFLAGS += -msse2 -funroll-loops -O3 -Wall # -mssse3
CXXFLAGS += `pkg-config --cflags opencv`
#CXXFLAGS += `pkg-config --cflags opencv-2.3.1`
LDFLAGS += -lboost_regex
#LDFLAGS += `pkg-config --libs opencv` -L /usr/lib/ -ljpeg


all: mkdir $(TARGETLIB) $(BINDIR)/$(TARGET)Comp $(BINDIR)/rtc.conf $(BINDIR)/$(TARGET)

.cpp.o:

mkdir:
	@if [ ! -d $(BINDIR) ]; then \
	echo "Make build directory";\
	mkdir -p $(BINDIR);\
	mkdir -p $(OBJDIR);\
	mkdir -p $(LIBDIR);\
	fi

$(BINDIR)/$(TARGET): $(OBJDIR)/$(TARGET)_main.o $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(TARGETLIB): $(OBJS)
	rm -f $@
	$(CXX) $(SHFLAGS) -o $@ $(OBJS) $(LDFLAGS)

$(TARGETCOMP): $(OBJDIR)/$(TARGET)Comp.o $(OBJS)
	$(CXX) -o $@ $(OBJDIR)/$(TARGET)Comp.o $(OBJS) $(LDFLAGS)

$(OBJDIR)/$(TARGETSERVICE)Comp.o: AppRecogConsumerComp.cpp AppRecogConsumer.cpp AppRecogConsumer.h  

clean: clean_objs
	rm -f *~
	rm -f *.lo
	rm -f *.la

dist-clean: clean_objs
	rm -rf ./build/

clean_objs:
	rm -f $(OBJS) $(OBJDIR)/$(TARGET)Comp.o $(TARGETLIB) $(TARGETCOMP)

# clean_skelstub:
# 	rm -f *Skel.h *Skel.cpp
# 	rm -f *Stub.h *Stub.cpp

ImgSkel.cpp : Img.idl
	$(IDLC) $(IDLFLAGS) -I/usr/include/rtm/idl Img.idl
	$(WRAPPER) $(WRAPPER_FLAGS) --idl-file=Img.idl
ImgSkel.h : Img.idl
	$(IDLC) $(IDLFLAGS) -I/usr/include/rtm/idl Img.idl
	$(WRAPPER) $(WRAPPER_FLAGS) --idl-file=Img.idl

ImgStub.cpp : Img.idl 
	$(IDLC) $(IDLFLAGS) Img.idl 
	$(WRAPPER) $(WRAPPER_FLAGS) --idl-file=Img.idl 
ImgStub.h : Img.idl 
	$(IDLC) $(IDLFLAGS) Img.idl 
	$(WRAPPER) $(WRAPPER_FLAGS) --idl-file=Img.idl 


VisionSkel.o: VisionSkel.cpp VisionSkel.h VisionStub.h
VisionStub.o: VisionStub.cpp VisionStub.h

VisionStub.cpp : Vision.idl 
	$(IDLC) $(IDLFLAGS) Vision.idl 
	$(WRAPPER) $(WRAPPER_FLAGS) --idl-file=Vision.idl 
VisionStub.h : Vision.idl 
	$(IDLC) $(IDLFLAGS) Vision.idl 
	$(WRAPPER) $(WRAPPER_FLAGS) --idl-file=Vision.idl 


$(OBJDIR)/$(TARGET).o: $(TARGET).cpp $(TARGET).h ImgSkel.h ImgSVC_impl.h
	rm -f $@
	$(CXX) $(CXXFLAGS) $(EXTRAINCS) -c -o $@ $<

$(OBJDIR)/$(TARGET)Comp.o: $(TARGET)Comp.cpp ImgSkel.h ImgSVC_impl.h
	rm -f $@
	$(CXX) $(CXXFLAGS) $(EXTRAINCS) -c -o $@ $<

$(OBJDIR)/$(TARGET)_main.o: $(TARGET)_main.cpp
	$(CXX) $(CXXFLAGS) $(EXTRAINCS) -c -o $@ $<

$(OBJDIR)/time.o: time.cpp
	$(CXX) $(CXXFLAGS) $(EXTRAINCS) -c -o $@ $<

$(OBJDIR)/simpsolib.o: simpsolib.cpp
	$(CXX) $(CXXFLAGS) $(EXTRAINCS) -c -o $@ $<

$(OBJDIR)/simtstlib.o: simtstlib.cpp
	$(CXX) $(CXXFLAGS) $(EXTRAINCS) -c -o $@ $<

$(BINDIR)/rtc.conf: rtc.conf
	cp rtc.conf $(BINDIR)/

$(OBJDIR)/ImgSVC_impl.o: ImgSVC_impl.cpp ImgSVC_impl.h ImgSkel.h ImgStub.h
	rm -f $@
	$(CXX) $(CXXFLAGS) $(EXTRAINCS) -c -o $@ $<
$(OBJDIR)/ImgSkel.o: ImgSkel.cpp ImgSkel.h ImgStub.h
	rm -f $@
	$(CXX) $(CXXFLAGS) $(EXTRAINCS) -c -o $@ $<
$(OBJDIR)/ImgStub.o: ImgStub.cpp ImgStub.h
	rm -f $@
	$(CXX) $(CXXFLAGS) $(EXTRAINCS) -c -o $@ $<

$(OBJDIR)/Vision.o: Vision.cpp
	$(CXX) $(CXXFLAGS) $(EXTRAINCS) -c -o $@ $<

$(OBJDIR)/VisionSVC_impl.o: VisionSVC_impl.cpp VisionSVC_impl.h VisionSkel.h VisionStub.h
	rm -f $@
	$(CXX) $(CXXFLAGS) $(EXTRAINCS) -c -o $@ $<
$(OBJDIR)/VisionSkel.o: VisionSkel.cpp VisionSkel.h VisionStub.h
	rm -f $@
	$(CXX) $(CXXFLAGS) $(EXTRAINCS) -c -o $@ $<
$(OBJDIR)/VisionStub.o: VisionStub.cpp VisionStub.h
	rm -f $@
	$(CXX) $(CXXFLAGS) $(EXTRAINCS) -c -o $@ $<


AppRecogConsumer.so: $(OBJS)



# end of Makefile
