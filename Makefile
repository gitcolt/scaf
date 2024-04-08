SRCS = scaf.c
SCAF_DIR = .scaf
SCAF_DIR_FULL = ${HOME}/${SCAF_DIR}
EXAMPLE_TEMPLATES_DIR = example_templates

.PHONY: all
all: scaf

scaf: $(SRCS)
	$(CC) -o $@ $^

.PHONY: install
install:
	if [ ! -d "${SCAF_DIR_FULL}" ]; then \
		mkdir -p ${SCAF_DIR_FULL}; \
		cp -R ${EXAMPLE_TEMPLATES_DIR}/* ${SCAF_DIR_FULL}; \
	fi
	sudo install ./scaf /usr/local/bin

.PHONY: install_completion
install_completion:
	sudo install ./scaf-completion /etc/bash_completion.d

.PHONY: install_all
install_all: install install_completion

.PHONY: uninstall_all
uninstall_all:
	sudo rm -i /usr/local/bin/scaf
	sudo rm -rI ${SCAF_DIR_FULL}
	sudo rm -i /etc/bash_completion.d/scaf-completion

.PHONY: clean
clean:
	rm -f ./scaf
