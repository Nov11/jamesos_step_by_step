dirs = $(shell ls -d */ | grep -v 'mnt')
all: 
	$(foreach N,$(dirs),make -C $(N);)	
update:all
	@sudo mount floppy.img mnt
	@sudo cp src/kernel mnt
	@sleep 1
	@sudo umount mnt
	@echo floppy img file updated
.PHONY:clean
clean:
	$(foreach N,$(dirs),make -C $(N) clean;)	
