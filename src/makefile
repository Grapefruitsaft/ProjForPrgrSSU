OUTDIR = ./bin
DATDIR = ./data
DATOUTDIR = ./bin/data
BUILD = project
OBJS = main.o fileIO.o login.o manageData.o menu.o
DATAS = book.txt borrow.txt client.txt

.PHONY: all clean

all : $(BUILD) 
	@cp -f $(addprefix $(DATDIR)/,$(DATAS)) $(DATOUTDIR)/

clean :
	@rm -f $(addprefix $(OUTDIR)/,$(BUILD)) $(addprefix $(OUTDIR)/,$(OBJS)) $(addprefix $(DATOUTDIR)/,$(DATAS))

$(BUILD) : $(OBJS) 
	gcc -Wall -o $(addprefix $(OUTDIR)/,$@) $(addprefix $(OUTDIR)/,$^)

main.o : $(subst .o,.c,$(notdir $@)) $(subst .o,.c,$(filter-out main.o,$(OBJS)))
%.o : $(subst .o,.c,$(notdir $@)) $(subst .o,.c,$(notdir $@))

.c.o :
	gcc -Wall -c -o $(addprefix $(OUTDIR)/,$@) $<
