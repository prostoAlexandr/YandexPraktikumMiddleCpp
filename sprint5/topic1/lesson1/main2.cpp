#include <algorithm>
#include <cassert>
#include <cstdio>
#include <functional>
#include <iostream>
#include <print>
#include <ranges>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <variant>
#include <vector>

namespace state {
struct WaitingForArrival {};

struct Docking {
    std::string gateNumber;
};

struct DisembarkingPassengers {
    int passengerCount;
};

struct GroundServiceInProgress {
    bool refuelingDone;
    bool baggageUnloaded;
    bool cleaningDone;
};

struct IncidentHandling {
    std::string issueDescription;
    bool critical;
};

struct GroundServiceFinished {};
}  // namespace state

using AircraftState =
    std::variant<state::WaitingForArrival, state::Docking, state::DisembarkingPassengers,
                 state::GroundServiceInProgress, state::IncidentHandling, state::GroundServiceFinished>;

namespace event {

struct PlaneLanded {
    std::string gateNumber;
};

struct GateConnected {
    std::string gateNumber;
};

struct ServicesCompleted {
    bool refueling;
    bool baggage;
    bool cleaning;
};

struct PassengersExited {
    int passengerCount;
};

struct IncidentDetected {
    std::string issueDescription;
    bool critical;
};

struct IncidentResolved {};

}  // namespace event

namespace E = event;
namespace S = state;
using AircraftEvent = std::variant<E::PlaneLanded, E::GateConnected, E::ServicesCompleted, E::PassengersExited,
                                   E::IncidentDetected, E::IncidentResolved>;

class AircraftStateVisitor {
public:
    AircraftState operator()(const S::WaitingForArrival &state, const E::PlaneLanded &evt) {
        std::println("Landed in gate: {}", evt.gateNumber);
        return S::Docking(evt.gateNumber);
    }

    AircraftState operator()(const S::Docking &state, const E::GateConnected &evt) {
        if (evt.gateNumber != state.gateNumber)
            throw std::logic_error("Несоответствие ворот при стыковке");
        std::println("Gate connected: {}", evt.gateNumber);
        return S::DisembarkingPassengers();
    }

    AircraftState operator()(const S::DisembarkingPassengers &state, const E::PassengersExited &evt) {
        std::println("Passengers exited: {}", evt.passengerCount);
        return S::GroundServiceInProgress();
    }

    AircraftState operator()(const S::GroundServiceInProgress &state, const E::ServicesCompleted &evt) {
        std::println("GroundServiceInProgress: refueling {}, baggage {}, cleaning {}", evt.refueling, evt.baggage,
                     evt.cleaning);
        auto [e0, e1, e2] = evt;
        std::array arr = {e0, e1, e2};
        if (std::ranges::all_of(arr, [](auto &&b) { return b; })) {
            return S::GroundServiceFinished();
        }
        return state;
    }

    AircraftState operator()(const S::IncidentHandling &state, const E::IncidentResolved &evt) {
        std::println("Incident Resolved");
        return S::GroundServiceInProgress();
    }

    AircraftState operator()(const S::GroundServiceInProgress &state, const E::IncidentDetected &evt) {
        std::println("Detected incident: {}, critical: {}", evt.issueDescription, evt.critical);
        return S::IncidentHandling();
    }

    AircraftState operator()(const auto &state, const auto &evt) {
        throw std::runtime_error(
            std::format("Not supported: state {}, event {}", typeid(state).name(), typeid(evt).name()));
    }
};

int main() {
    AircraftState currentState = S::WaitingForArrival{};

    std::vector<AircraftEvent> events = {
        E::PlaneLanded{"A12"},
        E::GateConnected{"A12"},
        E::PassengersExited{132},
        E::ServicesCompleted{true, true, false},
        E::IncidentDetected{"Утечка топлива", true},
        E::IncidentResolved{},
        E::ServicesCompleted{true, true, true},
    };

    for (const auto &event : events) {
        currentState = std::visit(AircraftStateVisitor{}, currentState, event);
    }
    assert(std::holds_alternative<S::GroundServiceFinished>(currentState));
}