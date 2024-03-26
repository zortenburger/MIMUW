#ifndef ORGANISM
#define ORGANISM

#include <stdint.h>
#include <optional>
#include <stdbool.h>
#include <tuple>

template<typename species_t, bool can_eat_meat, bool can_eat_plants> requires std::equality_comparable<species_t>
class Organism {
private:
    const species_t species;
    uint64_t vitality;
public:
    constexpr Organism(species_t const &species, uint64_t vitality) :
            species(species), vitality(vitality) {};

    constexpr uint64_t get_vitality() const {
        return vitality;
    }

    constexpr const species_t &get_species() const {
        return species;
    }

    constexpr void add_vitality(uint64_t vit) {
        this->vitality += vit;
    }

    constexpr void die() {
        this->vitality = 0;
    }

    constexpr bool is_dead() const {
        return (vitality == 0);
    }
};

template<typename species_t>
using Carnivore = Organism<species_t, true, false>;

template<typename species_t>
using Omnivore = Organism<species_t, true, true>;

template<typename species_t>
using Herbivore = Organism<species_t, false, true>;

template<typename species_t>
using Plant = Organism<species_t, false, false>;

namespace {
//changing order of first two tuple elements
    template<typename species_t, bool sp1_eats_m, bool sp1_eats_p,
            bool sp2_eats_m, bool sp2_eats_p>
    constexpr std::tuple<Organism<species_t, sp2_eats_m, sp2_eats_p>,
            Organism<species_t, sp1_eats_m, sp1_eats_p>,
            std::optional<Organism<species_t, sp2_eats_m, sp2_eats_p>>>
    swap_result(
            std::tuple<Organism<species_t, sp1_eats_m, sp1_eats_p>,
                    Organism<species_t, sp2_eats_m, sp2_eats_p>,
                    std::optional<Organism<species_t,
                            sp1_eats_m, sp1_eats_p>>> organisms) {
        return {get<1>(organisms), get<0>(organisms), {}};
    }

//checking if two organisms are same species
    template<typename species_t, bool sp1_eats_m, bool sp1_eats_p,
            bool sp2_eats_m, bool sp2_eats_p>
    constexpr bool
    sameSpecies(Organism<species_t, sp1_eats_m, sp1_eats_p> &organism1,
                Organism<species_t, sp2_eats_m, sp2_eats_p> &organism2) {
        return (sp1_eats_m == sp2_eats_m && sp1_eats_p == sp2_eats_p &&
                organism1.get_species() == organism2.get_species());
    }

//mating
    template<typename species_t, bool sp1_eats_m, bool sp1_eats_p,
            bool sp2_eats_m, bool sp2_eats_p>
    constexpr Organism<species_t, sp1_eats_m, sp1_eats_p> mating(
            Organism<species_t, sp1_eats_m, sp1_eats_p> &organism1,
            Organism<species_t, sp2_eats_m, sp2_eats_p> &organism2) {
        return Organism<species_t, sp1_eats_m, sp1_eats_p>(
                organism1.get_species(),
                (organism1.get_vitality() + organism2.get_vitality()) / 2);
    }

//organism1 eats herbivore
    template<typename species_t, bool sp1_eats_p>
    constexpr std::tuple<Organism<species_t, true, sp1_eats_p>, Herbivore<species_t>,
            std::optional<Organism<species_t, true, sp1_eats_p>>>
    encounter_worker(Organism<species_t, true, sp1_eats_p> &organism1,
                     Herbivore<species_t> &organism2) {
        if (organism1.get_vitality() > organism2.get_vitality()) {
            organism1.add_vitality(organism2.get_vitality() / 2);
            organism2.die();
        }
        return {organism1, organism2, {}};
    }

//organism2 eats herbivore
    template<typename species_t, bool sp2_eats_p>
    constexpr std::tuple<Herbivore<species_t>, Organism<species_t, true, sp2_eats_p>,
            std::optional<Herbivore<species_t>>>
    encounter_worker(Herbivore<species_t> &organism1,
                     Organism<species_t, true, sp2_eats_p> &organism2) {
        return swap_result(encounter_worker(organism2, organism1));
    }

//fight between organisms
    template<typename species_t, bool sp1_eats_p, bool sp2_eats_p>
    std::tuple<Organism<species_t, true, sp1_eats_p>,
            Organism<species_t, true, sp2_eats_p>,
            std::optional<Organism<species_t, true, sp1_eats_p>>>
    constexpr encounter_worker(Organism<species_t, true, sp1_eats_p> &organism1,
                               Organism<species_t, true, sp2_eats_p> &organism2) {
        if (sameSpecies(organism1, organism2)) {
            return {organism1, organism2, mating(organism1, organism2)};
        }
        if (organism1.get_vitality() > organism2.get_vitality()) {
            organism1.add_vitality(organism2.get_vitality() / 2);
            organism2.die();
        } else if (organism1.get_vitality() < organism2.get_vitality()) {
            organism2.add_vitality(organism1.get_vitality() / 2);
            organism1.die();
        } else {
            organism1.die();
            organism2.die();
        }
        return {organism1, organism2, {}};
    }

//carnivore meets plant
    template <typename species_t, bool sp1_eats_m>
    constexpr std::tuple<Organism<species_t, sp1_eats_m, false>,
            Plant<species_t>,
            std::optional<Organism<species_t, sp1_eats_m, false>>>
    encounter_worker(Organism<species_t, sp1_eats_m, false> &organism1,
                     Plant<species_t> &organism2) {
        return {organism1, organism2, {}};
    }

//plant meets carnivore
    template<typename species_t, bool sp1_eats_m>
    constexpr std::tuple<Plant<species_t>,
            Organism<species_t, sp1_eats_m, false>,
            std::optional<Plant<species_t>>>
    encounter_worker(Plant<species_t> &organism1,
                     Organism<species_t, sp1_eats_m, false> &organism2) {
        return swap_result(encounter_worker(organism2, organism1));
    }

//organism eating plants meet plant
    template<typename species_t, bool sp1_eats_m>
    constexpr std::tuple<Organism<species_t, sp1_eats_m, true>, Plant<species_t>,
            std::optional<Organism<species_t, sp1_eats_m, true>>>
    encounter_worker(Organism<species_t, sp1_eats_m, true> &organism1,
                     Plant<species_t> &organism2) {
        organism1.add_vitality(organism2.get_vitality());
        organism2.die();
        return {organism1, organism2, {}};
    }

// plant meet organism eating plants
    template<typename species_t, bool sp2_eats_m>
    constexpr std::tuple<Plant<species_t>, Organism<species_t, sp2_eats_m, true>,
            std::optional<Plant<species_t>>>
    encounter_worker(Plant<species_t> &organism1,
                     Organism<species_t, sp2_eats_m, true> &organism2) {
        return swap_result(encounter_worker(organism2, organism1));
    }

//two herbivores meet
    template<typename species_t>
    constexpr std::tuple<Herbivore<species_t>, Herbivore<species_t>,
            std::optional<Herbivore<species_t>>>
    encounter_worker(Herbivore<species_t> &organism1,
                     Herbivore<species_t> &organism2) {
        if (sameSpecies(organism1, organism2)) {
            return {organism1, organism2, mating(organism1, organism2)};
        }
        return {organism1, organism2, {}};
    }


    template<typename species_t, bool sp1_eats_m, bool sp1_eats_p,
            bool sp2_eats_m, bool sp2_eats_p, typename ... Args>
    constexpr Organism<species_t, sp1_eats_m, sp1_eats_p>
    encounter_series_helper(
            Organism<species_t, sp1_eats_m, sp1_eats_p> organism1,
            Organism<species_t, sp2_eats_m, sp2_eats_p> organism2,
            Args ... args) {
        if constexpr (sizeof...(args) > 0)
            return encounter_series_helper(
                    std::get<0>(encounter(organism1, organism2)), args ...);
        else {
            return std::get<0>(encounter(organism1, organism2));
        }
    }
}

template<typename species_t, bool sp1_eats_m, bool sp1_eats_p,
        bool sp2_eats_m, bool sp2_eats_p>
requires (sp1_eats_m || sp1_eats_p || sp2_eats_m || sp2_eats_p)
constexpr std::tuple<Organism<species_t, sp1_eats_m, sp1_eats_p>,
        Organism<species_t, sp2_eats_m, sp2_eats_p>,
        std::optional<Organism<species_t, sp1_eats_m, sp1_eats_p>>>
encounter(Organism<species_t, sp1_eats_m, sp1_eats_p> organism1,
          Organism<species_t, sp2_eats_m, sp2_eats_p> organism2) {
    if (organism1.is_dead() || organism2.is_dead()) {
        return {organism1, organism2, {}};
    }
    return encounter_worker(organism1, organism2);
}

template<typename species_t, bool sp1_eats_m, bool sp1_eats_p, typename ... Args>
constexpr Organism<species_t, sp1_eats_m, sp1_eats_p>
encounter_series(Organism<species_t, sp1_eats_m, sp1_eats_p> organism1,
                 Args ... args) {
    if constexpr (sizeof...(args) > 0)
        return encounter_series_helper(organism1, args ...);
    return organism1;
}

#endif 