.PHONY: clean All

All:
	@echo "----------Building project:[ vzray - Debug ]----------"
	@"$(MAKE)" -f  "vzray.mk"
clean:
	@echo "----------Cleaning project:[ vzray - Debug ]----------"
	@"$(MAKE)" -f  "vzray.mk" clean
