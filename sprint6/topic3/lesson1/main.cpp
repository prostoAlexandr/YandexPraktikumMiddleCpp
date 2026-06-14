#include <stop_token>
#include <atomic>

void worker(std::stop_token thread_token, std::stop_token external_token) {
    /*
    ваш код здесь
    */
    std::stop_source or_stop_source;
    std::stop_callback thread_cb(thread_token, [&or_stop_source]{
        or_stop_source.request_stop();
    });
    std::stop_callback external_cb(external_token, [&or_stop_source]{
        or_stop_source.request_stop();
    });

    third_party_library::some_useful_function(or_stop_source.get_token());
}