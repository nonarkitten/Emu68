
OBJS := sysinfo.o test-dhrystone.o topaz.o

OBJDIR := Build
TARGETDIR := ../../Build

all: $(TARGETDIR)/SysInfo

$(TARGETDIR)/SysInfo: $(addprefix $(OBJDIR)/, $(OBJS))
	@echo "Building target: $@"
	@$(M68K_CXX) $(foreach f,$(OBJS),$(OBJDIR)/$(f)) $(M68K_LDFLAGS) -o $@
	@echo "Build completed"

.PHONY: all

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(@D)
	@echo "Compiling: $*.cpp"
	$(M68K_CXX) -c $(M68K_CXXFLAGS) $< -o $@

$(OBJDIR)/%.d: %.cpp
	@mkdir -p $(@D)
	@set -e; rm -f $@; \
         $(M68K_CXX) -MM -MT $(basename $@).o $(M68K_CXXFLAGS) $< > $@.$$$$; \
         sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
         rm -f $@.$$$$

$(OBJDIR)/%.o: %.c
	@mkdir -p $(@D)
	@echo "Compiling: $*.c"
	$(M68K_CC) -c $(M68K_CFLAGS) $< -o $@

$(OBJDIR)/%.o: %.s
	@mkdir -p $(@D)
	@echo "Assembling: $*.c"
	$(M68K_CC) -c $(M68K_CFLAGS) $< -o $@

$(OBJDIR)/%.d: %.c
	@mkdir -p $(@D)
	@set -e; rm -f $@; \
         $(M68K_CC) -MM -MT $(basename $@).o $(M68K_CFLAGS) $< > $@.$$$$; \
         sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
         rm -f $@.$$$$

-include $(foreach f,$(OBJS:.o=.d),$(OBJDIR)/$(f))
