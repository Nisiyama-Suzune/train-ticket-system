#include "train_manager.h"

bool Train::selling() {
    return _selling;
}

void Train::change_selling() {
    _selling = !_selling;
}

void Train::add_remaining_tickets(int from, int to, KIND kind, int num) {
    for (int i = from; i < to; ++i) {
        station_available_ticket[i][kind] -= num;
    }
}

bool Train::have_ticket(int from, int to, KIND kind, int num) {
    for (int i = from; i < to; ++i) {
        if (station_available_ticket[i][kind] < num)
            return 0;
    }
    return 1;
}
