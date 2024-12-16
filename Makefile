
CPPFLAGS := -I/usr/include/lua5.4
CXXFLAGS := --std=c++20 -Wall -Werror

all: prerequisites dir srlua add
clean:
	@rm -rf b/* $(BT) srlua add
prerequisites:
	@which objcopy > /dev/null || echo "objcopy not installed (required to build ulutest)" || false
dir:
	@mkdir -p b

srlua: b/srlua.o b/runtime.o b/resources_linux.o
	@g++ -o $@ $(CPPFLAGS) $(CXXFLAGS) $^

b/srlua.o: src/srlua.cpp src/signature.h
	@g++ -c $< -o $@ $(CPPFLAGS) $(CXXFLAGS)

b/resources_linux.o: src/resources_linux.cpp src/signature.h
	@g++ -c $< -o $@ $(CPPFLAGS) $(CXXFLAGS)

b/runtime.o: b/runtime
	@objcopy -I binary -O elf64-x86-64\
		--redefine-sym _binary_b_runtime_start=runtime_start\
		--redefine-sym _binary_b_runtime_end=runtime_end $< $@
	@nm $@ > $(@:.o=.symbols)

b/runtime: src/runtime.cpp src/signature.h
	@g++ $< -o b/runtime $(CPPFLAGS) $(CXXFLAGS) -llua5.4

# ======================================================================

add: add.lua srlua
	@./srlua $< $@
	@chmod +x $@
