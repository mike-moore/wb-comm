#include "Navigation.h"

void Navigation::Execute() {
    /// - TODO Get data from sensors here. Perform any filtering prior
    ///   to sending it on for Guidance.
    State.SensedHeading = 45.0;
    State.SensedDistance = 5.0;
}

