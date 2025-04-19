DOXYFILE = docs/Doxyfile

docs:
	doxygen $(DOXYFILE)

docs-clean:
	@echo "$(RED)[CLEAN] Cleaning docs$(NO_COLOR)"
	@rm -rf build/docs

.PHONY: docs docs-clean