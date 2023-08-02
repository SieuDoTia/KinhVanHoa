CC=clang

danhSach = kinhVanHoa.o Mau.o PNG.o

kinhVanHoa: $(danhSach)
	$(CC) -o kinhVanHoa -lz -lm $(danhSach)

# ====
kinhVanHoa.o: kinhVanHoa.c
	$(CC) -c kinhVanHoa.c

Mau.o: Mau.c Mau.h
	$(CC) -c Mau.c

PNG.o: PNG.c PNG.h
	$(CC) -c PNG.c


# ==== LÀM SẠCH
clean:	
	rm $(danhSach)
