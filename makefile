CC=g++
SOURCE=stealer.cpp 
MYPROGRAM=ustealer
MYLIBRARIES=sqlite3
INSTALL_DIR = /usr/bin

all: $(MYPROGRAM)

$(MYPROGRAM): $(SOURCE)
	$(CC) $(SOURCE) -o$(MYPROGRAM) -l$(MYLIBRARIES)

clean:
	rm -f $(MYPROGRAM)

install :
	cp $(MYPROGRAM) $(INSTALL_DIR)
