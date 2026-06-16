// 1. Obtain the Awaiter object
auto&& value = <expr>;
auto&& awaiter = get_awaiter(promise, value); // via promise.await_transform or operator co_await

// 2. Check if the result is already available
if (!awaiter.await_ready()) 
{
    // --- Suspension Point Begins ---
    // The compiler saves local variables and the instruction pointer to the heap-allocated coroutine frame.

    using handle_t = std::coroutine_handle<PromiseType>;
    auto current_handle = handle_t::from_promise(promise);

    // 3. Delegate suspension logic (Handles void, bool, or another coroutine_handle)
    if constexpr (std::is_void_v<decltype(awaiter.await_suspend(current_handle))>) 
    {
        awaiter.await_suspend(current_handle);
        // Return control to the caller/resumer of the current coroutine
        return_to_caller_or_resumer(); 
    } 
    else if constexpr (std::is_same_v<decltype(awaiter.await_suspend(current_handle)), bool>) 
    {
        if (awaiter.await_suspend(current_handle)) 
        {
            return_to_caller_or_resumer();
        }
        // If false, the compiler skips suspension and drops straight down to resume immediately
    } 
    else 
    {
        // Symmetric transfer: immediately resume the returned coroutine handle without blowing up the stack
        auto next_handle = awaiter.await_suspend(current_handle);
        next_handle.resume(); 
    }

    // --- Resumption Point ---
    // When something calls current_handle.resume(), execution jumps right back here.
}

// 4. Fetch the final result
auto final_result = awaiter.await_resume();
