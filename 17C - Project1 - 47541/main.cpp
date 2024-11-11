#include "PokemonCard.h"
#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <random>
#include <string>

std::list<PokemonCard> opponentHand;
std::list<PokemonCard> opponentDeck;

// Function to apply Potion effect
void applyPotionEffect(PokemonCard &pokemon, int healingAmount) {
  pokemon.setHp(pokemon.getHp() + healingAmount);
  std::cout << "Applying Potion effect: Healing " << pokemon.getName()
            << " by " << healingAmount << " HP. New HP: " << pokemon.getHp()
            << "\n";
}

void applyDrawCardEffect(std::list<PokemonCard>& deck, std::list<PokemonCard>& hand, int drawCount = 1) {
    for (int i = 0; i < drawCount && !deck.empty(); ++i) {
        hand.push_back(deck.front());
        deck.pop_front();
    }
}

// Function to apply Poke Ball effect
void applyPokeBallEffect(std::list<PokemonCard> &deck,
                         std::list<PokemonCard> &hand) {
  auto it = std::find_if(deck.begin(), deck.end(), [](const PokemonCard &card) {
    return card.getCardType() == PokemonCard::CardType::Pokemon &&
           card.getStage() == 0;
  });
  if (it != deck.end()) {
    hand.push_back(*it);
    deck.erase(it);
    std::cout << "Applying Poke Ball effect: Added " << hand.back().getName()
              << " to hand.\n";
  } else {
    std::cout
        << "Applying Poke Ball effect: No Basic Pokemon found in the deck.\n";
  }
}

// Function to apply Sabrina effect
void applySabrinaEffect(std::list<PokemonCard> &opponentActive,
                         std::list<PokemonCard> &opponentBench) {
  if (!opponentBench.empty()) {
    std::cout
        << "Applying Sabrina effect: Opponent forced to switch active "
           "Pokemon.\n";

    // Display opponent's benched Pokemon
    std::cout << "Opponent's benched Pokemon:\n";
    int benchIndex = 1;
    for (const auto &card : opponentBench) {
      std::cout << benchIndex << ": " << card.getName() << "\n";
      benchIndex++;
    }

    // Get opponent's choice
    int choice;
    std::cout << "Choose a Pokemon to switch with (enter index 1-"
              << opponentBench.size() << "): ";
    std::cin >> choice;

    // Validate choice and perform the switch
    if (choice > 0 && choice <= opponentBench.size()) {
      auto it = opponentBench.begin();
      std::advance(it, choice - 1);

      // Swap using std::list::splice
      opponentActive.splice(opponentActive.begin(), opponentBench, it);

      std::cout << "Opponent switched their active Pokemon with "
                << opponentActive.front().getName() << ".\n";
    } else {
      std::cout << "Invalid choice. No switch performed.\n";
    }

  } else {
    std::cout << "Applying Sabrina effect: Opponent has no benched Pokemon.\n";
  }
}

// Function to apply Giovanni effect
void applyGiovanniEffect(int &attackDamage) {
  attackDamage += 10;
  std::cout << "Applying Giovanni effect: Adding 10 damage to the attack. New "
               "damage: "
            << attackDamage << "\n";
}

// Function to apply Red Card effect
void applyRedCardEffect(std::list<PokemonCard> &opponentHand,
                         std::list<PokemonCard> &opponentDeck) {
  std::cout
      << "Applying Red Card effect: Shuffling opponent's hand and drawing 3 "
         "cards.\n";

  // Shuffle opponent's hand back into their deck
  opponentDeck.splice(opponentDeck.end(), opponentHand);

  // Shuffle the deck
  std::random_device rd;  // Create a random device
  std::mt19937 gen(rd()); // Seed the Mersenne Twister generator with the random device
  std::shuffle(opponentDeck.begin(), opponentDeck.end(), gen);

  // Draw 3 cards (if possible)
  int cardsToDraw = std::min(3, (int)opponentDeck.size());
  for (int i = 0; i < cardsToDraw; ++i) {
    opponentHand.push_back(opponentDeck.front());
    opponentDeck.pop_front();
  }
}

// Function to apply Professor's Research effect
void applyProfessorsResearchEffect(std::list<PokemonCard> &deck,
                                   std::list<PokemonCard> &hand,
                                   int drawAmount) {
  if (deck.size() >= drawAmount) {
    std::cout << "Applying Professor's Research effect: Drawing " << drawAmount
              << " cards.\n";
    for (int i = 0; i < drawAmount; ++i) {
      hand.push_back(deck.front());
      deck.pop_front();
    }
  } else {
    std::cout
        << "Applying Professor's Research effect: Not enough cards in the deck "
           "to draw.\n";
  }
}

// Function to apply Meowth's Pay Day attack effect
void applyPayDayEffect(std::list<PokemonCard> &deck,
                        std::list<PokemonCard> &hand) {
  if (!deck.empty()) {
    hand.push_back(deck.front());
    deck.pop_front();
    std::cout << "Applying Pay Day effect: Drew a card: "
              << hand.back().getName() << std::endl;
  } else {
    std::cout << "Applying Pay Day effect: No cards left in the deck to draw!\n";
  }
}

// Function to apply Mewtwo ex's Psydrive attack effect
void applyPsydriveEffect(PokemonCard &attacker) {
  // Check if Mewtwo has enough Psychic energy to discard
  if (attacker.getAttachedEnergy().at("Psychic") >= 2) {
    // Discard 2 Psychic energy from attacker
    attacker.removeEnergy("Psychic", 2);
    std::cout << "Applying Psydrive effect: Discarded 2 Psychic energy from "
              << attacker.getName() << ".\n";
  } else {
    std::cout << "Psydrive cannot be used. Not enough Psychic energy attached "
                 "to "
              << attacker.getName() << ".\n";
  }
}

// Function to apply Gardevoir's Psy Shadow ability effect
void applyPsyShadowEffect(PokemonCard &activePokemon, PokemonCard &gardevoir) {
  if (!gardevoir.isPsyShadowUsed()) {
    // Add 1 Psychic energy to activePokemon
    activePokemon.attachEnergy("Psychic");
    std::cout << "Applying Psy Shadow effect: Added 1 Psychic energy to "
              << activePokemon.getName() << ".\n";
    gardevoir.setPsyShadowUsed(true);
  } else {
    std::cout << "Psy Shadow has already been used this turn.\n";
  }
}

// Function to apply Charmander's Ember attack effect
void applyEmberEffect(PokemonCard &attacker) {
  // Check if Charmander has enough Fire energy to discard
  if (attacker.getAttachedEnergy().at("Fire") >= 1) {
    // Discard 1 Fire energy from attacker
    attacker.removeEnergy("Fire");
    std::cout << "Applying Ember effect: Discarded 1 Fire energy from "
              << attacker.getName() << ".\n";
  } else {
    std::cout << "Ember cannot be used. Not enough Fire energy attached to "
              << attacker.getName() << ".\n";
  }
}

// Function to apply Charizard ex's Crimson Storm attack effect
void applyCrimsonStormEffect(PokemonCard &attacker) {
  // Check if Charizard ex has enough Fire energy to discard
  if (attacker.getAttachedEnergy().at("Fire") >= 2) {
    // Discard 2 Fire energy from attacker
    attacker.removeEnergy("Fire", 2);
    std::cout << "Applying Crimson Storm effect: Discarded 2 Fire energy from "
              << attacker.getName() << ".\n";
  } else {
    std::cout
        << "Crimson Storm cannot be used. Not enough Fire energy attached to "
        << attacker.getName() << ".\n";
  }
}

// Function to apply Pikachu ex's Circle Circuit attack effect
void applyCircleCircuitEffect(const std::list<PokemonCard> &bench,
                              int &attackDamage) {
  attackDamage *= bench.size(); // Multiply base damage by the number of benched
                                // Pokemon
  std::cout << "Applying Circle Circuit effect: Damage increased to "
            << attackDamage << " due to benched Pokemon.\n";
}

// Function to apply Zapdos ex's Thundering Hurricane attack effect
void applyThunderingHurricaneEffect(int &attackDamage) {
  std::random_device rd;  // Create a random device
  std::mt19937 gen(rd()); // Seed the Mersenne Twister generator with the random device
  std::uniform_int_distribution<> distrib(0, 1); // Distribution for coin flip (0
                                                // or 1)
  int headsCount = 0;
  for (int i = 0; i < 4; ++i) {
    if (distrib(gen) == 1) { // 1 represents heads
      headsCount++;
    }
  }
  attackDamage *= headsCount; // Multiply base damage by the number of heads
  std::cout << "Applying Thundering Hurricane effect: Flipped 4 coins, got "
            << headsCount << " heads. Damage is now " << attackDamage << ".\n";
}

// Function to apply Moltres ex's Inferno Dance ability effect
void applyInfernoDanceEffect(std::list<PokemonCard> &bench,
                             std::string &energyType) {
  std::random_device rd;  // Create a random device
  std::mt19937 gen(rd()); // Seed the Mersenne Twister generator with the random device
  std::uniform_int_distribution<> distrib(0, 1); // Distribution for coin flip (0
                                                // or 1)

  int headsCount = 0;
  for (int i = 0; i < 3; ++i) {
    if (distrib(gen) == 1) { // 1 represents heads
      headsCount++;
    }
  }

  if (headsCount > 0 && !bench.empty()) {
    std::cout << "Applying Inferno Dance effect: Flipped 3 coins, got "
              << headsCount
              << " heads. Choose benched Pokemon to add Fire energy to:\n";

    // Display benched Pokemon
    int benchIndex = 0;
    std::map<int, PokemonCard *> benchedPokemon;
    for (auto &card : bench) {
      std::cout << benchIndex + 1 << ": " << card.getName() << "\n";
      benchedPokemon[benchIndex] = &card;
      benchIndex++;
    }

    // Get player's choice for each head
    for (int i = 0; i < headsCount; ++i) {
      std::cout << "Choose a Pokemon to attach Fire energy to (enter index): ";
      int choice;
      std::cin >> choice;
      if (choice > 0 && choice <= bench.size()) {
        auto it = benchedPokemon.find(choice - 1);
        if (it != benchedPokemon.end()) {
          it->second->attachEnergy(energyType);
          std::cout << "Attached 1 Fire energy to " << it->second->getName()
                    << ".\n";
        }
      } else {
        std::cout << "Invalid choice. Skipping energy attachment.\n";
      }
    }
  } else {
    if (headsCount == 0) {
      std::cout << "Applying Inferno Dance effect: Flipped 3 coins, got no "
                   "heads.\n";
    } else {
      std::cout << "Applying Inferno Dance effect: No benched Pokemon to "
                   "attach energy to.\n";
    }
  }
}

bool checkLoseCondition(const std::list<PokemonCard> &active,
                        const std::list<PokemonCard> &bench) {
  if (active.empty() && bench.empty()) {
    return true; // Lose if no active and no benched Pokemon
  }
  return false;
}

void playerTurn(std::list<PokemonCard> &playerDeck,
                 std::list<PokemonCard> &playerHand,
                 std::list<PokemonCard> &playerActive,
                 std::list<PokemonCard> &playerBench,
                 std::list<PokemonCard> &opponentDeck,
                 std::list<PokemonCard> &opponentHand,
                 std::list<PokemonCard> &opponentActive,
                 std::list<PokemonCard> &opponentBench, int &playerPoints,
                 int &aiPoints, std::string &playerEnergyType,
                 int &playerEnergy) {

  std::cout << "\nPlayer's turn:\n";

  // Reset Psy Shadow flag for Gardevoir
  for (auto &card : playerDeck) {
    if (card.getName() == "Gardevoir") {
      card.setPsyShadowUsed(false);
    }
  }
  for (auto &card : playerHand) {
    if (card.getName() == "Gardevoir") {
      card.setPsyShadowUsed(false);
    }
  }
  for (auto &card : playerActive) {
    if (card.getName() == "Gardevoir") {
      card.setPsyShadowUsed(false);
    }
  }
  for (auto &card : playerBench) {
    if (card.getName() == "Gardevoir") {
      card.setPsyShadowUsed(false);
    }
  }

  // Draw a card
  if (!playerDeck.empty()) {
    playerHand.push_back(playerDeck.front());
    playerDeck.pop_front();
    std::cout << "Drew a card: " << playerHand.back().getName() << std::endl;
  } else {
    std::cout << "No cards left in the deck to draw!\n";
  }

  // Generate energy
  playerEnergy += 1;
  std::cout << "Generated 1 " << playerEnergyType << " energy. You now have "
            << playerEnergy << " energy.\n";

  // Play cards (interactive logic)
bool playedSupporter = false;
for (auto it = playerHand.begin(); it != playerHand.end();) {
    if (it->getCardType() == PokemonCard::CardType::Item) {
        if (it->getItemEffectType() == PokemonCard::ItemEffect::ItemEffectType::Heal &&
            !playerActive.empty()) {

            if (auto healingAmount = it->getHealingAmount(); healingAmount.has_value()) {
                applyPotionEffect(playerActive.front(), *healingAmount); // Dereference the optional
            }

            it = playerHand.erase(it);
        } else if (it->getItemEffectType() == PokemonCard::ItemEffect::ItemEffectType::Search) {
            applyPokeBallEffect(playerDeck, playerHand);
            it = playerHand.erase(it);
        } else if (it->getItemEffectType() == PokemonCard::ItemEffect::ItemEffectType::Shuffle) {
            applyRedCardEffect(opponentHand, opponentDeck);
            it = playerHand.erase(it);
        } else {
            ++it;
        }
    } else if (!playedSupporter &&
               it->getCardType() == PokemonCard::CardType::Supporter) {
        if (it->getSupporterEffectType() == PokemonCard::SupporterEffect::SupporterEffectType::DrawCard) {
            if (auto drawAmount = it->getDrawAmount(); drawAmount.has_value()) {
                applyProfessorsResearchEffect(playerDeck, playerHand, *drawAmount); // Dereference the optional
            }
            it = playerHand.erase(it);
        } else if (it->getName() == "Sabrina") {
            applySabrinaEffect(opponentActive, opponentBench);
            it = playerHand.erase(it);
        } else if (it->getName() == "Giovanni") {
            // Giovanni effect is applied during the attack phase
            ++it;
        } else {
            ++it; // Skip other Supporters for now
        }
        playedSupporter = true;
    } else {
        ++it;
    }
}


  // Play Pokemon (interactive logic)
  std::cout << "Your hand:\n";
  int i = 0;
  for (const auto &card : playerHand) {
    std::cout << i << ": " << card.getName() << " (" << card.getType() << ") HP: "
              << card.getHp() << "\n";
    i++;
  }
  std::cout << "Choose a Pokemon to play (enter index, or -1 to skip): ";
  int choice;
  std::cin >> choice;
  if (choice >= 0 && choice < playerHand.size()) {
    auto it = playerHand.begin();
    std::advance(it, choice); // Move iterator to the chosen position

    if (playerActive.empty()) {
        playerActive.push_back(*it);
        std::cout << "Played " << playerActive.back().getName()
                  << " to the active spot.\n";
    } else if (playerBench.size() < 3) {
        playerBench.push_back(*it);
        std::cout << "Played " << playerBench.back().getName()
                  << " to the bench.\n";
    } else {
        std::cout << "Bench is full.\n";
    }
    
    playerHand.erase(it); // Remove the chosen card from playerHand
} else {
    std::cout << "Invalid choice. Please select a valid card index.\n";
}

  // Evolve Pokemon (interactive logic)
  std::cout << "Pokemon that can evolve:\n";
  int evolveIndex = 0;
  std::map<int, PokemonCard *> evolvablePokemon;
  for (auto &card : playerActive) {
    if (card.getCardType() == PokemonCard::CardType::Pokemon &&
        card.getStage() < 2) {
      std::cout << evolveIndex << ": " << card.getName() << " (Active)\n";
      evolvablePokemon[evolveIndex] = &card;
      evolveIndex++;
    }
  }
  for (auto &card : playerBench) {
    if (card.getCardType() == PokemonCard::CardType::Pokemon &&
        card.getStage() < 2) {
      std::cout << evolveIndex << ": " << card.getName() << " (Bench)\n";
      evolvablePokemon[evolveIndex] = &card;
      evolveIndex++;
    }
  }
  if (!evolvablePokemon.empty()) {
    std::cout << "Choose a Pokemon to evolve (enter index, or -1 to skip): ";
    int choice;
    std::cin >> choice;
    if (choice >= 0 && choice < evolveIndex) {
      PokemonCard *pokemonToEvolve = evolvablePokemon[choice];
      std::cout << "Choose a card from your hand to evolve "
                << pokemonToEvolve->getName() << " into: \n";
      int handIndex = 0;
      std::map<int, std::list<PokemonCard>::iterator> evolutionChoices;
      for (auto it = playerHand.begin(); it != playerHand.end(); ++it) {
        if (it->getCardType() == PokemonCard::CardType::Pokemon &&
            it->getStage() == pokemonToEvolve->getStage() + 1) {
          std::cout << handIndex << ": " << it->getName() << "\n";
          evolutionChoices[handIndex] = it;
          handIndex++;
        }
      }
      if (!evolutionChoices.empty()) {
        std::cin >> choice;
        if (choice >= 0 && choice < handIndex) {
          auto evolutionIt = evolutionChoices[choice];
          if (pokemonToEvolve == &playerActive.front()) {
            playerActive.front() = *evolutionIt;
          } else {
            for (auto &card : playerBench) {
              if (&card == pokemonToEvolve) {
                card = *evolutionIt;
                break;
              }
            }
          }
          playerHand.erase(evolutionIt);
          std::cout << "Evolved " << pokemonToEvolve->getName() << " into "
                    << evolutionChoices[choice]->getName() << ".\n";
        } else {
          std::cout << "Invalid choice. Skipping evolution.\n";
        }
      } else {
        std::cout << "No suitable evolutions found in your hand.\n";
      }
    }
  }

  // Attach energy (interactive logic)
  if (playerEnergy > 0) {
    std::cout << "Choose a Pokemon to attach energy to:\n";
    int energyIndex = 0;
    std::map<int, PokemonCard *> energyTargets;
    for (auto &card : playerActive) {
      if (card.getCardType() == PokemonCard::CardType::Pokemon) {
        std::cout << energyIndex << ": " << card.getName() << " (Active)\n";
        energyTargets[energyIndex] = &card;
        energyIndex++;
      }
    }
    for (auto &card : playerBench) {
      if (card.getCardType() == PokemonCard::CardType::Pokemon) {
        std::cout << energyIndex << ": " << card.getName() << " (Bench)\n";
        energyTargets[energyIndex] = &card;
        energyIndex++;
      }
    }
    if (!energyTargets.empty()) {
      std::cout
          << "Choose a Pokemon to attach energy to (enter index, or -1 to "
             "skip): ";
      int choice;
      std::cin >> choice;
      if (choice >= 0 && choice < energyIndex) {
        PokemonCard *energyTarget = energyTargets[choice];

        energyTarget->attachEnergy(playerEnergyType);
        std::cout << "Attached 1 " << playerEnergyType << " energy to "
                  << energyTarget->getName() << ".\n";

        playerEnergy--;
      }
    }
  }

  // Attack (interactive logic) - Only active Pokemon can attack
if (!playerActive.empty() && !opponentActive.empty()) {
    PokemonCard *attacker = &playerActive.front();
    std::cout << "What will " << attacker->getName() << " do?\n";
    std::cout << "1. Attack\n";
    if (!playerBench.empty()) {
        std::cout << "2. Retreat\n";
    }
    int choice;
    std::cin >> choice;
    if (choice == 1) {
        std::cout << "Choose an attack:\n";
        int attackChoice = 0;
        std::map<int, std::map<std::string, int>::const_iterator> attackChoices; // Use const_iterator
        for (auto it = attacker->getAttacks().begin(); it != attacker->getAttacks().end(); ++it) {
            if (attacker->hasEnoughEnergy(it->first, playerEnergyType)) {
                std::cout << attackChoice + 1 << ": " << it->first << "\n";
                attackChoices[attackChoice] = it;
                attackChoice++;
            }
        }
        if (attackChoices.empty()) {
            std::cout << "Not enough energy to attack.\n";
        } else {
            std::cin >> choice;
            if (choice > 0 && choice <= attackChoices.size()) {
                auto attackIt = attackChoices[choice - 1];
                int attackDamage = attackIt->second;

                PokemonCard *defender = &opponentActive.front();
                std::cout << attacker->getName() << " used " << attackIt->first
                          << " on " << defender->getName() << "!\n";

          // Apply attack effects
          if (attacker->getName() == "Meowth" &&
              attackIt->first == "Pay Day") {
            applyPayDayEffect(playerDeck, playerHand);
          } else if (attacker->getName() == "Mewtwo ex" &&
                     attackIt->first == "Psydrive") {
            applyPsydriveEffect(*attacker);
          } else if (attacker->getName() == "Charmander" &&
                     attackIt->first == "Ember") {
            applyEmberEffect(*attacker);
          } else if (attacker->getName() == "Charizard ex" &&
                     attackIt->first == "Crimson Storm") {
            applyCrimsonStormEffect(*attacker);
          }

          // Apply weakness
          if (defender->getWeakness() == attacker->getType()) {
            attackDamage += 20;
            std::cout << "It's super effective! (Weakness)\n";
          }

          defender->setHp(defender->getHp() - attackDamage);
          std::cout << defender->getName() << " takes " << attackDamage
                    << " damage. HP remaining: " << defender->getHp() << "\n";

          // Check for knock out
          if (defender->getHp() <= 0) {
            std::cout << defender->getName() << " is knocked out!\n";
            if (defender->getName().find("ex") != std::string::npos) {
              playerPoints += 2;
            } else {
              playerPoints += 1;
            }
            std::cout << "Player gains points. Current points: "
                      << playerPoints << "\n";
            opponentActive.pop_front();

            if (!opponentBench.empty()) {
              std::cout << "Who will you send out?\n";
              int benchIndex = 0;
              for (const auto &card : opponentBench) {
                std::cout << benchIndex + 1 << ": " << card.getName() << "\n";
                benchIndex++;
              }
              int choice;
              std::cin >> choice;
              if (choice > 0 && choice <= opponentBench.size()) {
                auto it = opponentBench.begin();
                std::advance(it, choice - 1);
                opponentActive.push_front(
                    *it); // Move chosen Pokemon to active
                opponentBench.erase(it); // Remove from bench
              }
            } else {
              std::cout << "Opponent has no Pokemon to send out! Player wins!\n";
              playerPoints = 3;
            }
          }
        } else {
          std::cout << "Invalid attack choice.\n";
        }
      }
    } else if (choice == 2) {
      // Retreat logic
      std::cout << "Which Pokemon will you swap with?\n";
      int benchIndex = 0;
      std::map<int, std::list<PokemonCard>::iterator> benchChoices;
      for (auto it = playerBench.begin(); it != playerBench.end(); ++it) {
        std::cout << benchIndex + 1 << ": " << it->getName() << "\n";
        benchChoices[benchIndex] = it;
        benchIndex++;
      }
      int choice;
      std::cin >> choice;
      if (choice > 0 && choice <= benchChoices.size()) {
        // Swap the active Pokemon with the chosen benched Pokemon
        auto benchIt = benchChoices[choice - 1];
        std::swap(playerActive.front(), *benchIt);
      } else {
        std::cout << "Invalid choice. Skipping retreat.\n";
      }
    }
  }

  // Check lose condition
  if (checkLoseCondition(playerActive, playerBench)) {
    std::cout << "Player loses!\n";
    aiPoints = 3;
  }

  // Check win condition
  if (playerPoints >= 3 || checkLoseCondition(opponentActive, opponentBench)) {
    std::cout << "Player wins!\n";
    playerPoints = 3;
  }
}

void aiTurn(std::list<PokemonCard> &aiDeck, std::list<PokemonCard> &aiHand,
             std::list<PokemonCard> &aiActive,
             std::list<PokemonCard> &aiBench,
             std::list<PokemonCard> &playerDeck,
             std::list<PokemonCard> &playerHand,
             std::list<PokemonCard> &playerActive,
             std::list<PokemonCard> &playerBench, int &aiPoints,
             int &playerPoints, std::string &aiEnergyType, int &aiEnergy) {

  std::cout << "\nAI's turn:\n";

  // Draw a card
  if (!aiDeck.empty()) {
    aiHand.push_back(aiDeck.front());
    aiDeck.pop_front();
    std::cout << "AI drew a card.\n";
  } else {
    std::cout << "AI has no cards left in the deck to draw!\n";
  }

  // Generate energy
  aiEnergy += 1;
  std::cout << "AI generated 1 " << aiEnergyType << " energy.\n";

  // Play Supporter card (simplified logic)
for (auto it = aiHand.begin(); it != aiHand.end(); ++it) {
    if (it->getCardType() == PokemonCard::CardType::Supporter) {
        if (it->getName() == "Professor's Research") {
            if (auto drawAmount = it->getDrawAmount(); drawAmount.has_value()) {
                applyProfessorsResearchEffect(aiDeck, aiHand, *drawAmount);
            }
        } else if (it->getName() == "Sabrina") {
            applySabrinaEffect(playerActive, playerBench);
        } else if (it->getName() == "Giovanni") {
            // Giovanni effect is applied during the attack phase
        }
        aiHand.erase(it);
        break; // Only play one Supporter per turn
    }
}

  // Play Item card (simplified logic)
for (auto it = aiHand.begin(); it != aiHand.end(); ++it) {
    if (it->getCardType() == PokemonCard::CardType::Item) {
        if (auto itemEffectType = it->getItemEffectType(); itemEffectType.has_value()) {
            if (*itemEffectType == PokemonCard::ItemEffect::ItemEffectType::Heal) {
                applyPotionEffect(playerActive.front(), it->getHealingAmount().value_or(0));
            } else if (*itemEffectType == PokemonCard::ItemEffect::ItemEffectType::DrawCard) {
                applyDrawCardEffect(aiDeck, aiHand);
            } else if (*itemEffectType == PokemonCard::ItemEffect::ItemEffectType::Search) {
                applyPokeBallEffect(aiDeck, aiHand);
            } else if (*itemEffectType == PokemonCard::ItemEffect::ItemEffectType::Shuffle) {
                applyRedCardEffect(opponentHand, opponentDeck); // No scope error now
            }
        }
        aiHand.erase(it);
        break; // Only play one Item per turn
    }
}

  // Play Pokemon (simplified logic)
  if (aiActive.empty()) {
    for (auto it = aiHand.begin(); it != aiHand.end(); ++it) {
      if (it->getCardType() == PokemonCard::CardType::Pokemon &&
          it->getStage() == 0) {
        aiActive.push_back(*it);
        aiHand.erase(it);
        std::cout << "AI played " << aiActive.back().getName()
                  << " to the active spot.\n";
        break;
      }
    }
  }

  for (auto it = aiHand.begin(); it != aiHand.end(); ++it) {
    if (it->getCardType() == PokemonCard::CardType::Pokemon &&
        it->getStage() == 0 && aiBench.size() < 3) {
      aiBench.push_back(*it);
      aiHand.erase(it);
      std::cout << "AI played " << aiBench.back().getName()
                << " to the bench.\n";
    }
  }

  // Evolve Pokemon (simplified logic)
  for (auto activeIt = aiActive.begin(); activeIt != aiActive.end();
       ++activeIt) {
    if (activeIt->getCardType() == PokemonCard::CardType::Pokemon &&
        activeIt->getStage() < 2) {
      for (auto handIt = aiHand.begin(); handIt != aiHand.end(); ++handIt) {
        if (handIt->getCardType() == PokemonCard::CardType::Pokemon &&
            handIt->getStage() == activeIt->getStage() + 1 &&
            handIt->getType() ==
                activeIt->getType()) { // Check if the Pokemon in hand is the
                                       // next stage and same type

          std::cout << "AI evolved " << activeIt->getName() << " into "
                    << handIt->getName() << ".\n";
          *activeIt = *handIt; // Replace the active Pokemon with the evolved
                                // one
          aiHand.erase(handIt);
          break;
        }
      }
    }
  }
  // Evolve Pokemon on the bench (simplified logic)
  for (auto benchIt = aiBench.begin(); benchIt != aiBench.end(); ++benchIt) {
    if (benchIt->getCardType() == PokemonCard::CardType::Pokemon &&
        benchIt->getStage() < 2) {
      for (auto handIt = aiHand.begin(); handIt != aiHand.end(); ++handIt) {
        if (handIt->getCardType() == PokemonCard::CardType::Pokemon &&
            handIt->getStage() == benchIt->getStage() + 1 &&
            handIt->getType() ==
                benchIt->getType()) { // Check if the Pokemon in hand is the
                                       // next stage and same type

          std::cout << "AI evolved " << benchIt->getName() << " into "
                    << handIt->getName() << ".\n";
          *benchIt = *handIt; // Replace the benched Pokemon with the evolved
                                // one
          aiHand.erase(handIt);
          break;
        }
      }
    }
  }

  // Attach energy (simplified logic)
  if (aiEnergy > 0 && !aiActive.empty()) {
    aiActive.front().attachEnergy(aiEnergyType);
    std::cout << "AI attached 1 " << aiEnergyType << " energy to "
              << aiActive.front().getName() << ".\n";
    aiEnergy--;
  }

  // Attack (simplified logic)
  if (!aiActive.empty() && !playerActive.empty()) {
    PokemonCard *attacker = &aiActive.front();
    PokemonCard *defender = &playerActive.front();

    auto attackIt = attacker->getAttacks().begin();
    int attackDamage = attackIt->second;

    std::cout << "AI's " << attacker->getName() << " used "
              << attackIt->first << " on " << defender->getName() << "!\n";

    if (defender->getWeakness() == attacker->getType()) {
      attackDamage += 20;
      std::cout << "It's super effective! (Weakness)\n";
    }

    defender->setHp(defender->getHp() - attackDamage);
    std::cout << defender->getName() << " takes " << attackDamage
              << " damage. HP remaining: " << defender->getHp() << "\n";
    
    if (defender->getHp() <= 0) {
      std::cout << defender->getName() << " is knocked out!\n";
      if (defender->getName().find("ex") != std::string::npos) {
        aiPoints += 2;
      } else {
        aiPoints += 1;
      }
      std::cout << "AI gains points. Current points: " << aiPoints << "\n";
      playerActive.pop_front();

      if (!playerBench.empty()) {
        playerActive.push_front(playerBench.front());
        playerBench.pop_front();
      }
    }
  }

  // Check lose condition
  if (checkLoseCondition(aiActive, aiBench)) {
    std::cout << "AI loses!\n";
    playerPoints = 3;
  }

  // Check win condition
  if (aiPoints >= 3 || checkLoseCondition(playerActive, playerBench)) {
    std::cout << "AI wins!\n";
    aiPoints = 3;
  }
  
}

int main(void) {
  // Mewtwo EX deck creation
  std::list<PokemonCard> mewtwoExDeck;

  // Mewtwo ex x2
  PokemonCard mewtwoEx{
      "Mewtwo ex", "Psychic", 150, PokemonCard::CardType::Pokemon};
  mewtwoEx.addAttack("Psychic Sphere", 50, 1,
                     1); // 1 psychic, 1 colorless
  mewtwoEx.addAttack("Psydrive", 150, 2, 2); // 2 psychic, 2 colorless
  mewtwoEx.setWeakness("Dark");
  mewtwoEx.setStage(0);          // Basic Pokemon
  mewtwoEx.setRetreatCost(2);
  mewtwoExDeck.push_back(mewtwoEx);
  mewtwoExDeck.push_back(mewtwoEx);

  // Ralts x2
  PokemonCard ralts{"Ralts", "Psychic", 60, PokemonCard::CardType::Pokemon};
  ralts.addAttack("Ram", 10, 0, 1); // 1 colorless
  ralts.setWeakness("Dark");
  ralts.setStage(0);          // Basic Pokemon
  ralts.setRetreatCost(1);
  mewtwoExDeck.push_back(ralts);
  mewtwoExDeck.push_back(ralts);

  // Kirlia x2
  PokemonCard kirlia{"Kirlia", "Psychic", 80, PokemonCard::CardType::Pokemon};
  kirlia.addAttack("Smack", 30, 1, 1); // 1 psychic, 1 colorless
  kirlia.setWeakness("Dark");
  kirlia.setStage(1);          // Stage 1 Pokemon
  kirlia.setRetreatCost(1);
  mewtwoExDeck.push_back(kirlia);
  mewtwoExDeck.push_back(kirlia);

  // Gardevoir x2
  PokemonCard gardevoir{"Gardevoir", "Psychic", 110,
                        PokemonCard::CardType::Pokemon};
  gardevoir.addAttack("Psyshot", 60, 2, 1); // 2 psychic, 1 colorless
  gardevoir.setWeakness("Dark");
  gardevoir.setStage(2);          // Stage 2 Pokemon
  gardevoir.setRetreatCost(2);
  mewtwoExDeck.push_back(gardevoir);
  mewtwoExDeck.push_back(gardevoir);

  // Potion x2
  PokemonCard potion{"Potion", "Item", 0, PokemonCard::CardType::Item};
  potion.effect.itemEffect.itemEffectType = PokemonCard::ItemEffectType::Heal;
  potion.effect.itemEffect.healingAmount = 20;
  mewtwoExDeck.push_back(potion);
  mewtwoExDeck.push_back(potion);

  // X Speed x2
  PokemonCard xSpeed{"X Speed", "Item", 0, PokemonCard::CardType::Item};
  xSpeed.effect.itemEffect.itemEffectType = PokemonCard::ItemEffectType::Other;
  mewtwoExDeck.push_back(xSpeed);
  mewtwoExDeck.push_back(xSpeed);

  // Poke Ball x2
  PokemonCard pokeBall{"Poke Ball", "Item", 0, PokemonCard::CardType::Item};
  pokeBall.effect.itemEffect.itemEffectType =
      PokemonCard::ItemEffectType::Search;
  mewtwoExDeck.push_back(pokeBall);
  mewtwoExDeck.push_back(pokeBall);

  // Red Card x1
  PokemonCard redCard{"Red Card", "Item", 0, PokemonCard::CardType::Item};
  redCard.effect.itemEffect.itemEffectType =
      PokemonCard::ItemEffectType::Shuffle;
  mewtwoExDeck.push_back(redCard);

  // Professor's Research x2
  PokemonCard professorsResearch{"Professor's Research", "Supporter", 0,
                                 PokemonCard::CardType::Supporter};
  professorsResearch.effect.supporterEffect.supporterEffectType =
      PokemonCard::SupporterEffectType::DrawCard;
  professorsResearch.effect.supporterEffect.drawAmount = 2;
  mewtwoExDeck.push_back(professorsResearch);
  mewtwoExDeck.push_back(professorsResearch);

  // Sabrina x1
  PokemonCard sabrina{"Sabrina", "Supporter", 0,
                      PokemonCard::CardType::Supporter};
  sabrina.effect.supporterEffect.supporterEffectType =
      PokemonCard::SupporterEffectType::Other;
  mewtwoExDeck.push_back(sabrina);

  // Giovanni x2
  PokemonCard giovanni{"Giovanni", "Supporter", 0,
                      PokemonCard::CardType::Supporter};
  giovanni.effect.supporterEffect.supporterEffectType =
      PokemonCard::SupporterEffectType::Other;
  mewtwoExDeck.push_back(giovanni);
  mewtwoExDeck.push_back(giovanni);
}
  // Pikachu EX deck creation
  std::list<PokemonCard> pikachuExDeck;

  // Pikachu ex x2
  PokemonCard pikachuEx{"Pikachu ex", "Electric", 120,
                        PokemonCard::CardType::Pokemon};
  pikachuEx.addAttack("Circle Circuit", 30, 2, 0); // 2 lightning
  pikachuEx.setWeakness("Fighting");
  pikachuEx.setStage(0);          // Basic Pokemon
  pikachuEx.setRetreatCost(1);
  pikachuExDeck.push_back(pikachuEx);
  pikachuExDeck.push_back(pikachuEx);

  // Zapdos ex x2
  PokemonCard zapdosEx{"Zapdos ex", "Electric", 130,
                        PokemonCard::CardType::Pokemon};
  zapdosEx.addAttack("Peck", 20, 1, 0);           // 1 lightning
  zapdosEx.addAttack("Thundering Hurricane", 50, 3, 0); // 3 lightning
  zapdosEx.setWeakness("Lightning");
  zapdosEx.setStage(0);          // Basic Pokemon
  zapdosEx.setRetreatCost(1);
  pikachuExDeck.push_back(zapdosEx);
  pikachuExDeck.push_back(zapdosEx);

  // Voltorb x2
  PokemonCard voltorb{"Voltorb", "Electric", 60,
                      PokemonCard::CardType::Pokemon};
  voltorb.addAttack("Tackle", 20, 1, 0); // 1 lightning
  voltorb.setWeakness("Fighting");
  voltorb.setStage(0);          // Basic Pokemon
  voltorb.setRetreatCost(1);
  pikachuExDeck.push_back(voltorb);
  pikachuExDeck.push_back(voltorb);

  // Electrode x2
  PokemonCard electrode{"Electrode", "Electric", 80,
                        PokemonCard::CardType::Pokemon};
  electrode.addAttack("Electro Ball", 70, 2, 0); // 2 lightning
  electrode.setWeakness("Fighting");
  electrode.setStage(1);          // Stage 1 Pokemon
  electrode.setRetreatCost(0);
  pikachuExDeck.push_back(electrode);
  pikachuExDeck.push_back(electrode);

  // Poke Ball x2
  PokemonCard pokeBall2{"Poke Ball", "Item", 0, PokemonCard::CardType::Item};
  pokeBall2.effect.itemEffect.itemEffectType =
      PokemonCard::ItemEffectType::Search;
  pikachuExDeck.push_back(pokeBall2);
  pikachuExDeck.push_back(pokeBall2);

  // Potion x2
  PokemonCard potion2{"Potion", "Item", 0, PokemonCard::CardType::Item};
  potion2.effect.itemEffect.itemEffectType = PokemonCard::ItemEffectType::Heal;
  potion2.effect.itemEffect.healingAmount = 20;
  pikachuExDeck.push_back(potion2);
  pikachuExDeck.push_back(potion2);

  // Red Card x1
  PokemonCard redCard2{"Red Card", "Item", 0, PokemonCard::CardType::Item};
  redCard2.effect.itemEffect.itemEffectType =
      PokemonCard::ItemEffectType::Shuffle;
  pikachuExDeck.push_back(redCard2);

  // X Speed x2
  PokemonCard xSpeed2{"X Speed", "Item", 0, PokemonCard::CardType::Item};
  xSpeed2.effect.itemEffect.itemEffectType = PokemonCard::ItemEffectType::Other;
  pikachuExDeck.push_back(xSpeed2);
  pikachuExDeck.push_back(xSpeed2);

  // Sabrina x2
  PokemonCard sabrina2{"Sabrina", "Supporter", 0,
                       PokemonCard::CardType::Supporter};
  sabrina2.effect.supporterEffect.supporterEffectType =
      PokemonCard::SupporterEffectType::Other;
  pikachuExDeck.push_back(sabrina2);
  pikachuExDeck.push_back(sabrina2);

  // Professor's Research x2
  PokemonCard professorsResearch2{"Professor's Research", "Supporter", 0,
                                  PokemonCard::CardType::Supporter};
  professorsResearch2.effect.supporterEffect.supporterEffectType =
      PokemonCard::SupporterEffectType::DrawCard;
  professorsResearch2.effect.supporterEffect.drawAmount = 2;
  pikachuExDeck.push_back(professorsResearch2);
  pikachuExDeck.push_back(professorsResearch2);

  // Giovanni x1
  PokemonCard giovanni2{"Giovanni", "Supporter", 0,
                       PokemonCard::CardType::Supporter};
  giovanni2.effect.supporterEffect.supporterEffectType =
      PokemonCard::SupporterEffectType::Other;
  pikachuExDeck.push_back(giovanni2);
  
  // Charizard EX deck creation
  std::list<PokemonCard> charizardExDeck;

  // Charmander x2
  PokemonCard charmander{"Charmander", "Fire", 60,
                        PokemonCard::CardType::Pokemon};
  charmander.addAttack("Ember", 30, 1, 0); // 1 fire
  charmander.setWeakness("Water");
  charmander.setStage(0);          // Basic Pokemon
  charmander.setRetreatCost(1);
  charizardExDeck.push_back(charmander);
  charizardExDeck.push_back(charmander);

  // Charmeleon x2
  PokemonCard charmeleon{"Charmeleon", "Fire", 90,
                        PokemonCard::CardType::Pokemon};
  charmeleon.addAttack("Fire Claws", 60, 1, 2); // 1 fire, 2 colorless
  charmeleon.setWeakness("Water");
  charmeleon.setStage(1);          // Stage 1 Pokemon
  charmeleon.setRetreatCost(2);
  charizardExDeck.push_back(charmeleon);
  charizardExDeck.push_back(charmeleon);

  // Charizard ex x2
  PokemonCard charizardEx{"Charizard ex", "Fire", 180,
                          PokemonCard::CardType::Pokemon};
  charizardEx.addAttack("Slash", 60, 1, 2);     // 1 fire, 2 colorless
  charizardEx.addAttack("Crimson Storm", 200, 2, 2); // 2 fire, 2 colorless
  charizardEx.setWeakness("Water");
  charizardEx.setStage(2);          // Stage 2 Pokemon
  charizardEx.setRetreatCost(2);
  charizardExDeck.push_back(charizardEx);
  charizardExDeck.push_back(charizardEx);

  // Moltres ex x2
  PokemonCard moltresEx{"Moltres ex", "Fire", 140,
                        PokemonCard::CardType::Pokemon};
  moltresEx.addAttack("Heat Blast", 70, 1, 2); // 1 fire, 2 colorless
  moltresEx.setWeakness("Lightning");
  moltresEx.setStage(0);          // Basic Pokemon
  moltresEx.setRetreatCost(2);
  charizardExDeck.push_back(moltresEx);
  charizardExDeck.push_back(moltresEx);

  // Meowth x1
  PokemonCard meowth{"Meowth", "Colorless", 60, PokemonCard::CardType::Pokemon};
  meowth.addAttack("Pay Day", 10, 0, 1); // 1 colorless
  meowth.setWeakness("Fighting");
  meowth.setStage(0);          // Basic Pokemon
  meowth.setRetreatCost(1);
  charizardExDeck.push_back(meowth);

  // Sabrina x2
  PokemonCard sabrina3{"Sabrina", "Supporter", 0,
                       PokemonCard::CardType::Supporter};
  sabrina3.effect.supporterEffect.supporterEffectType =
      PokemonCard::SupporterEffectType::Other;
  charizardExDeck.push_back(sabrina3);
  charizardExDeck.push_back(sabrina3);

  // Potion x2
  PokemonCard potion3{"Potion", "Item", 0, PokemonCard::CardType::Item};
  potion3.effect.itemEffect.itemEffectType = PokemonCard::ItemEffectType::Heal;
  potion3.effect.itemEffect.healingAmount = 20;
  charizardExDeck.push_back(potion3);
  charizardExDeck.push_back(potion3);

  // X Speed x2
  PokemonCard xSpeed3{"X Speed", "Item", 0, PokemonCard::CardType::Item};
  xSpeed3.effect.itemEffect.itemEffectType = PokemonCard::ItemEffectType::Other;
  charizardExDeck.push_back(xSpeed3);
  charizardExDeck.push_back(xSpeed3);

  // Poke Ball x2
  PokemonCard pokeBall3{"Poke Ball", "Item", 0, PokemonCard::CardType::Item};
  pokeBall3.effect.itemEffect.itemEffectType =
      PokemonCard::ItemEffectType::Search;
  charizardExDeck.push_back(pokeBall3);
  charizardExDeck.push_back(pokeBall3);

  // Red Card x1
  PokemonCard redCard3{"Red Card", "Item", 0, PokemonCard::CardType::Item};
  redCard3.effect.itemEffect.itemEffectType =
      PokemonCard::ItemEffectType::Shuffle;
  charizardExDeck.push_back(redCard3);

  // Professor's Research x2
  PokemonCard professorsResearch3{"Professor's Research", "Supporter", 0,
                                  PokemonCard::CardType::Supporter};
  professorsResearch3.effect.supporterEffect.supporterEffectType =
  PokemonCard::SupporterEffectType::DrawCard;
  professorsResearch3.effect.supporterEffect.drawAmount = 2;
  charizardExDeck.push_back(professorsResearch3);
  charizardExDeck.push_back(professorsResearch3);
  
  // Choose decks and deal hands
  std::cout << "Choose your deck (1: Mewtwo EX, 2: Pikachu EX, 3: Charizard EX): ";
  int deckChoice;
  std::cin >> deckChoice;

  list<PokemonCard> playerDeck, opponentDeck;
  std::string playerEnergyType, aiEnergyType;

  switch (deckChoice) {
  case 1:
    playerDeck = mewtwoExDeck;
    playerEnergyType = "Psychic";
    // AI gets a random deck
    if (rand() % 2 == 0) {
      opponentDeck = pikachuExDeck;
      aiEnergyType = "Lightning";
    } else {
      opponentDeck = charizardExDeck;
      aiEnergyType = "Fire";
    }
    break;
  case 2:
    playerDeck = pikachuExDeck;
    playerEnergyType = "Lightning";
    // AI gets a random deck
    if (rand() % 2 == 0) {
      opponentDeck = mewtwoExDeck;
      aiEnergyType = "Psychic";
    } else {
      opponentDeck = charizardExDeck;
      aiEnergyType = "Fire";
    }
    break;
  case 3:
    playerDeck = charizardExDeck;
    playerEnergyType = "Fire";
    // AI gets a random deck
    if (rand() % 2 == 0) {
      opponentDeck = mewtwoExDeck;
      aiEnergyType = "Psychic";
    } else {
      opponentDeck = pikachuExDeck;
      aiEnergyType = "Lightning";
    }
    break;
  default:
    std::cout << "Invalid choice. Exiting.\n";
    return 1;
  }

  list<PokemonCard> playerHand;
  list<PokemonCard> playerActive;
  list<PokemonCard> playerBench;

  list<PokemonCard> opponentHand;
  list<PokemonCard> opponentActive;
  list<PokemonCard> opponentBench;

  // Shuffle decks
  std::random_device rd;  // Create a random device
  std::mt19937 gen(rd()); // Seed the Mersenne Twister generator with the random device
  shuffle(playerDeck.begin(), playerDeck.end(), g);
  shuffle(opponentDeck.begin(), opponentDeck.end(), g);

  // Deal starting hands
  for (int i = 0; i < 5; ++i) {
    playerHand.push_back(playerDeck.front());
    playerDeck.pop_front();
    opponentHand.push_back(opponentDeck.front());
    opponentDeck.pop_front();
  }

  // Coin toss
  std::cout << "Coin toss... ";
  bool playerTurn = (rand() % 2 == 0);
  if (playerTurn) {
    std::cout << "Player goes first!\n";
  } else {
    std::cout << "AI goes first!\n";
  }

  int playerPoints = 0;
  int aiPoints = 0;
  int playerEnergy = 0;
  int aiEnergy = 0;
  
  // Game loop (simplified)
  while (playerPoints < 3 && aiPoints < 3) {
    if (playerTurn) {
      playerTurn(playerDeck, playerHand, playerActive, playerBench,
                 opponentDeck, opponentHand, opponentActive, opponentBench,
                 playerPoints, aiPoints, playerEnergyType, playerEnergy);
    } else {
      aiTurn(opponentDeck, opponentHand, opponentActive, opponentBench,
                 playerDeck, playerHand, playerActive, playerBench,
                 aiPoints, playerPoints, aiEnergyType, aiEnergy);
    }

    playerTurn = !playerTurn; // Switch turns
  }

  // Determine winner
  if (playerPoints >= 3) {
    std::cout << "Player wins!\n";
  } else {
    std::cout << "AI wins!\n";
  }
return 0;
}