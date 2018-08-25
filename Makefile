DEBUG?=1

ifeq ($(DEBUG),0)
  override CMAKE_OPTIONS+=-DCMAKE_BUILD_TYPE=Release
else
  override CMAKE_OPTIONS+=-DCMAKE_BUILD_TYPE=Debug
endif


BUILD_DIR:=./build
INSTALL_PREFIX:=/
CMAKE_OPTIONS+=-DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX}
BUILD_DIR_ABS:=$(shell pwd)/$(BUILD_DIR)

all: $(BUILD_DIR)/Makefile
	$(MAKE) -C $(BUILD_DIR) --no-print-directory
	@echo "Build complete in directory $(BUILD_DIR)"


$(BUILD_DIR)/Makefile:
	mkdir -p $(BUILD_DIR)
	( cd $(BUILD_DIR) && cmake .. $(CMAKE_OPTIONS) )
	
clean:
	rm -f $(BUILD_DIR)/Makefile
	find $(BUILD_DIR) | grep runner_out.xml | xargs -r rm
