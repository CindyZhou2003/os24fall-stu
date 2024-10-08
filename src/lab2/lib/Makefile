# 获取 lib 目录下的所有 C 文件
C_SRC = $(sort $(wildcard *.c))
OBJ = $(patsubst %.c,%.o,$(C_SRC))

# 默认目标
all: $(OBJ)

# 编译 C 文件
%.o: %.c
	${GCC} ${CFLAG} -c $< -o $@

# 清理编译生成的文件
clean:
	$(shell rm -f *.o)

