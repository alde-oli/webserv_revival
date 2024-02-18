all:
	@echo "\n\n[Nouveau message reçu]\nKan lé condés sont dans la rue, sa pu sa, sa pu duc\n\n"

zbeb:
	@echo "\n\n[Message de zbeb]\nJ'koné une meuf narvalo, elle habite à Metz, bah elle est plus fraiche que toua\n\n"

surpriz:
	@echo "Choisissez le kdo 1 ou le kdo 2: "; \
	read number; \
	if [ "$$number" = "1" ]; then \
		echo "\n🎊 Félicitations vous avez gagné une visite sur le chantier d'une autoroute 🥳"; \
	elif [ "$$number" = "2" ]; then \
		echo "\n✨ Félicitations vous venez de gagner une semaine de vacances tout frais payés dans un bidonville 🎉"; \
    else \
		echo "Entrée non reconnue : $$number"; \
    fi
