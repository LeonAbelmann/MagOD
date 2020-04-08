CC=g++ #Compiler
CFLAGS=-I fileStdio/include #Compiler directives

# Macro for MagOD object files
OBJ_RECIPES =
#OBJ_RECIPES = src/recipes/recipes.o src/led/led.o

# Macro for object files
OBJ = TestRecipes.o fileStdio/IO.o $(OBJ_RECIPES)

# Macro for MagOD includes files:
DEPS_RECIPES=
#DEPS_RECIPES = src/recipes/recipes.h src/led/led.h 

# Macro for all dependencies
DEPS = TestRecipes.h fileStdio/IO.h $(DEPS_RECIPES)

# All cpp files depend on the includes, so recompile their objects:
%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# Dependencies of TestRecipes and what to include
TestRecipes: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
