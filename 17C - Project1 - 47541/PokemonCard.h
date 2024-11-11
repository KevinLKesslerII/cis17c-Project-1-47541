#ifndef POKEMONCARD_H
#define POKEMONCARD_H

#include <iostream>
#include <map>
#include <string>
#include <variant>
#include <optional>

class PokemonCard {
public:
    enum class CardType { Pokemon, Item, Supporter };

    struct PokemonEffect {
        int stage;
        std::map<std::string, std::pair<int, int>> attackEnergyCosts;
        int retreatCost;
        bool psyShadowUsed;
         std::map<std::string, std::map<std::string, int>> attackEnergyRequirements; // attackName -> {energyType -> amount}
    };

    struct ItemEffect {
        int healingAmount;
        enum class ItemEffectType { Heal, DrawCard, Search, Other, Shuffle, DamageBoost } itemEffectType;
    };
    
    struct SupporterEffect {
        int drawAmount;
        enum class SupporterEffectType { DrawCard, Search, Shuffle, Other } supporterEffectType;
    };

    using Effect = std::variant<PokemonEffect, ItemEffect, SupporterEffect>;

    PokemonCard(const std::string& name, const std::string& type, int hp = 0,
                CardType cardType = CardType::Pokemon)
        : name(name), type(type), weakness(weakness), hp(hp), cardType(cardType), stage(0) {
        if (cardType == CardType::Pokemon) {
            effect = PokemonEffect{0, {}, 0, false};
        } else if (cardType == CardType::Item) {
            effect = ItemEffect{0, ItemEffect::ItemEffectType::Other};
        } else if (cardType == CardType::Supporter) {
            effect = SupporterEffect{0, SupporterEffect::SupporterEffectType::Other};
        }
    }

    int getHp() const { return hp; }
    void setHp(int newHp) { hp = newHp; }
    std::string getName() const { return name; }
    std::string getType() const { return type; }
    std::string getWeakness() const { return weakness; }
    CardType getCardType() const { return cardType; }
    int getStage() const { return stage; }
    void setStage(int newStage) { stage = newStage; }
    
     // Attacks
    const std::map<std::string, int>& getAttacks() const { return attacks; }
    void addAttack(const std::string& attackName, int damage, const std::map<std::string, int>& energyRequirements) {
        attacks[attackName] = damage;
        if (std::holds_alternative<PokemonEffect>(effect)) {
            std::get<PokemonEffect>(effect).attackEnergyRequirements[attackName] = energyRequirements;
        }
    }
    const std::map<std::string, int>& getAttachedEnergy() const { return attachedEnergy; }
    void attachEnergy(const std::string& energyType) { attachedEnergy[energyType]++; }
    void removeEnergy(const std::string& energyType, int amount = 1) {
        if (attachedEnergy[energyType] >= amount) {
            attachedEnergy[energyType] -= amount;
            if (attachedEnergy[energyType] == 0) {
                attachedEnergy.erase(energyType);
            }
        }
    }
    
    bool hasEnoughEnergy(const std::string& attackName, const std::string& playerEnergyType) const {
        if (std::holds_alternative<PokemonEffect>(effect)) {
            const auto& energyCost = std::get<PokemonEffect>(effect).attackEnergyCosts;
            auto it = energyCost.find(attackName);
            if (it != energyCost.end()) {
                int psychicCost = it->second.first;
                int colorlessCost = it->second.second;
                
                int attachedPsychic = attachedEnergy.count("Psychic") ? attachedEnergy.at("Psychic") : 0;
                int attachedPlayerEnergy = attachedEnergy.count(playerEnergyType) ? attachedEnergy.at(playerEnergyType) : 0;

                return (attachedPsychic >= psychicCost && attachedPlayerEnergy >= colorlessCost);
            }
        }
        return false;
    }

    bool isPsyShadowUsed() const {
        if (std::holds_alternative<PokemonEffect>(effect)) {
            return std::get<PokemonEffect>(effect).psyShadowUsed;
        }
        return false;
    }

    void setPsyShadowUsed(bool used) {
        if (std::holds_alternative<PokemonEffect>(effect)) {
            std::get<PokemonEffect>(effect).psyShadowUsed = used;
        }
    }
    

    std::optional<ItemEffect::ItemEffectType> getItemEffectType() const {
        if (std::holds_alternative<ItemEffect>(effect)) {
            return std::get<ItemEffect>(effect).itemEffectType;
        }
        return std::nullopt;
    }

    std::optional<SupporterEffect::SupporterEffectType> getSupporterEffectType() const {
        if (std::holds_alternative<SupporterEffect>(effect)) {
            return std::get<SupporterEffect>(effect).supporterEffectType;
        }
        return std::nullopt;
    }

    std::optional<int> getHealingAmount() const {
        if (std::holds_alternative<ItemEffect>(effect)) {
            return std::get<ItemEffect>(effect).healingAmount;
        }
        return std::nullopt;
    }

    std::optional<int> getDrawAmount() const {
        if (std::holds_alternative<SupporterEffect>(effect)) {
            return std::get<SupporterEffect>(effect).drawAmount;
        }
        return std::nullopt;
    }

private:
    std::string name;
    std::string type;  // Pokemon type (e.g., Fire, Water)
    std::string weakness;
    int hp;
    CardType cardType;
    int stage;
    std::map<std::string, int> attacks;
    std::map<std::string, int> attachedEnergy;
    Effect effect;
};

#endif // POKEMONCARD_H
