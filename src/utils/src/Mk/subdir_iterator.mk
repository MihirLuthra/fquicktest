.PHONY: all_subdir all_subdir_clean

all_subdir:
	@for subdir in $(SUBDIR) ; do          \
	    echo ---;                          \
	    echo --- Making in $$subdir;       \
	    echo ---;                          \
	    ( cd $$subdir && make ) || exit 1; \
	done

all_subdir_clean:
	@for subdir in $(SUBDIR) ; do       \
	    echo ---;                       \
	    echo --- Cleaning in $$subdir;  \
	    echo ---;                       \
	    ( cd $$subdir && make clean )   \
	done
