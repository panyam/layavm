
include Makefile.common

.PHONY: all libs tests install
all: libs tests


libs: src
	@cd src ; make static shared

tests: libs
	@cd tests ; make

.PHONY: clean cleanall distclean package
clean:
	@cd src ; make clean
	@cd tests ; make clean
	@rm -Rf $(OUTPUT_DIR)

cleanall: clean
	@cd src ; make cleanall
	@cd tests ; make cleanall
	@rm -Rf $(OUTPUT_DIR)

distclean: cleanall
	@cd src ; make distclean
	@cd tests ; make distclean
	@rm -Rf $(OUTPUT_DIR)

