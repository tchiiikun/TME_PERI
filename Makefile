CARD_NUMB       = 24
ROUTER          = 132.227.102.36
LOGIN           = lee-fabre

INSDEV 		= insdev
RMDEV 		= rmdev

rd:;         scp -P50$(CARD_NUMB) $(RMDEV) pi@$(ROUTER):$(LOGIN)
id:;        scp -P50$(CARD_NUMB) $(INSDEV) pi@$(ROUTER):$(LOGIN)
