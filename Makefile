############################################
#
# Basse85 Test
#   copyright 2019.07.11 konoar
#
############################################

TARGETBIN := ./testbin

.PHONY: clean run

run: $(TARGETBIN)
	@-$(TARGETBIN)

clean:
	@-rm $(TARGETBIN)

$(TARGETBIN): main.c
	@gcc -o $(TARGETBIN) main.c

