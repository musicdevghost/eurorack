# gMock Cheat Sheet

## Defining a Mock Class

### Mocking a Normal Class {#MockClass}

Given

```cpp
class Foo {
  ...
  virtual ~Foo();
  virtual int GetSize() const = 0;
  virtual string Describe(const char* name) = 0;
  virtual string Describe(int type) = 0;
  virtual bool Process(Bar elem, int count) = 0;
};
```

(note that `~Foo()` **must** be virtual) we can define its mock as

```cpp
#include "gmock/gmock.h"

class MockFoo : public Foo {
  ...
  MOCK_METHOD(int, GetSize, (), (const, override));
  MOCK_METHOD(string, Describe, (const char* name), (override));
  MOCK_METHOD(string, Describe, (int type), (override));
  MOCK_METHOD(bool, Process, (Bar elem, int count), (override));
};
```

To create a "nice" mock, which ignores all uninteresting calls, a "naggy" mock,
which warns on all uninteresting calls, or a "strict" mock, which treats them as
failures:

```cpp
using ::testing::NiceMock;
using ::testing::NaggyMock;
using ::testing::StrictMock;

NiceMock<MockFoo> nice_foo;      // The type is a subclass of MockFoo.
NaggyMock<MockFoo> naggy_foo;    // The type is a subclass of MockFoo.
StrictMock<MockFoo> strict_foo;  // The type is a subclass of MockFoo.
```

{: .callout .note}
**Note:** A mock object is currently naggy by default. We may make it nice by
default in the future.

### Mocking a Class Template {#MockTemplate}

Class templates can be mocked just like any class.

To mock

```cpp
template <typename Elem>
class StackInterface {
  ...
  virtual ~StackInterface();
  virtual int GetSize() const = 0;
  virtual void Push(const Elem& x) = 0;
};
```

(note that all member functions that are mocked, including `~StackInterface()`
**must** be virtual).

```cpp
template <typename Elem>
class MockStack : public StackInterface<Elem> {
  ...
  MOCK_METHOD(int, GetSize, (), (const, override));
  MOCK_METHOD(void, Push, (const Elem& x), (override));
};
```

### Specifying Calling Conventions for Mock Functions

If your mock function doesn't use the default calling convention, you can
specify it by adding `Calltype(convention)` to `MOCK_METHOD`'s 4th parameter.
For example,

```cpp
  MOCK_METHOD(bool, Foo, (int n), (Calltype(STDMETHODCALLTYPE)));
  MOCK_METHOD(int, Bar, (double x, double y),
              (const, Calltype(STDMETHODCALLTYPE)));
```

where `STDMETHODCALLTYPE` is defined by `<objbase.h>` on Windows.

## Using Mocks in Tests {#UsingMocks}

The typical work flow is:

1.  Import the gMock names you need to use. All gMock symbols are in the
    `testing` namespace unless they are macros or otherwise noted.
2.  Create the mock objects.
3.  Optionally, set the default actions of the mock objects.
4.  Set your expectations on the mock objects (How will they be called? What
    will they do?).
5.  Exercise code that uses the mock objects; if necessary, check the result
    using googletest assertions.
6.  When a mock object is destructed, gMock automatically verifies that all
    expectations on it have been satisfied.

Here's an example:

```cpp
using ::testing::Return;                          // #1

TEST(BarTest, DoesThis) {
  MockFoo foo;                                    // #2

  ON_CALL(foo, GetSize())                         // #3
      .WillByDefault(Return(1));
  // ... other default actions ...

  EXPECT_CALL(foo, Describe(5))                   // #4
      .Times(3)
      .WillRepeatedly(Return("Category 5"));
  // ... other expectations ...

  EXPECT_EQ(MyProductionFunction(&foo), "good");  // #5
}                                                 // #6
```

## Setting Default Actions {#OnCall}

gMock has a **built-in default action** for any function that returns `void`,
`bool`, a numeric value, or a pointer. In C++11, it will additionally returns
the default-constructed value, if one exists for the given type.

To customize the default action for functions with return type *`T`*:

```cpp
using ::testing::DefaultValue;

// Sets the default value to be returned. T must be CopyConstructible.
DefaultValue<T>::Set(value);
// Sets a factory. Will be invoked on demand. T must be MoveConstructible.
//  T MakeT();
DefaultValue<T>::SetFactory(&MakeT);
// ... use the mocks ...
// Resets the default value.
DefaultValue<T>::Clear();
```

Example usage:

```cpp
  // Sets the default action for return type std::unique_ptr<Buzz> to
  // creating a new Buzz every time.
  DefaultValue<std::unique_ptr<Buzz>>::SetFactory(
      [] { return MakeUnique<Buzz>(AccessLevel::kInternal); });

  // When this fires, the default action of MakeBuzz() will run, which
  // will return a new Buzz object.
  EXPECT_CALL(mock_buzzer_, MakeBuzz("hello")).Times(AnyNumber());

  auto buzz1 = mock_buzzer_.MakeBuzz("hello");
  auto buzz2 = mock_buzzer_.MakeBuzz("hello");
  EXPECT_NE(buzz1, nullptr);
  EXPECT_NE(buzz2, nullptr);
  EXPECT_NE(buzz1, buzz2);

  // Resets the default action for return type std::unique_ptr<Buzz>,
  // to avoid interfere with other tests.
  DefaultValue<std::unique_ptr<Buzz>>::Clear();
```

To customize the default action for a particular method of a specific mock
object, use `ON_CALL()`. `ON_CALL()` has a similar syntax to `EXPECT_CALL()`,
but it is used for setting default behaviors (when you do not require that the
mock method is called). See [here](gmock_cook_book.md#UseOnCall) for a more
detailed discussion.

```cpp
ON_CALL(mock-object, method(matchers))
    .With(multi-argument-matcher)   ?
    .WillByDefault(action);
```

## Setting Expectations {#ExpectCall}

`EXPECT_CALL()` sets **expectations** on a mock method (How will it be called?
What will it do?):

```cpp
EXPECT_CALL(mock-object, method (matchers)?)
     .With(multi-argument-matcher)  ?
     .Times(cardinality)            ?
     .InSequence(sequences)         *
     .After(expectations)           *
     .WillOnce(action)              *
     .WillRepeatedly(action)        ?
     .RetiresOnSaturation();        ?
```

For each item above, `?` means it can be used at most once, while `*` means it
can be used any number of times.

In order to pass, `EXPECT_CALL` must be used before the calls are actually made.

The `(matchers)` is a comma-separated list of matchers that correspond to each
of the arguments of `method`, and sets the expectation only for calls of
`method` that matches all of the matchers.

If `(matchers)` is omitted, the expectation is the same as if the matchers were
set to anything matchers (for example, `(_, _, _, _)` for a four-arg method).

If `Times()` is omitted, the cardinality is assumed to be:

*   `Times(1)` when there is neither `WillOnce()` nor `WillRepeatedly()`;
*   `Times(n)` when there are `n` `WillOnce()`s but no `WillRepeatedly()`, where
    `n` >= 1; or
*   `Times(AtLeast(n))` when there are `n` `WillOnce()`s and a
    `WillRepeatedly()`, where `n` >= 0.

A method with no `EXPECT_CALL()` is free to be invoked *any number of times*,
and the default action will be taken each time.

## Matchers {#MatcherList}

See the [Matchers Reference](reference/matchers.md).

## Actions {#ActionList}

**Actions** specify what a mock function should do when invoked.

### Returning a Value

|                                   |                                               |
| :-------------------------------- | :-------------------------------------------- |
| `Return()`                        | Return from a `void` mock function.           |
| `Return(value)`                   | Return `value`. If the type of `value` is     different to the mock function's return type, `value` is converted to the latter type <i>at the time the expectation is set</i>, not when the action is executed. |
| `ReturnArg<N>()`                  | Return the `N`-th (0-based) argument.         |
| `ReturnNew<T>(a1, ..., ak)`       | Return `new T(a1, ..., ak)`; a different      object is created each time. |
| `ReturnNull()`                    | Return a null pointer.                        |
| `ReturnPointee(ptr)`              | Return the value pointed to by `ptr`.         |
| `ReturnRef(variable)`             | Return a reference to `variable`.             |
| `ReturnRefOfCopy(value)`          | Return a reference to a copy of `value`; the  copy lives as long as the action. |
| `ReturnRoundRobin({a1, ..., ak})` | Each call will return the next `ai` in the list, starting at the beginning when the end of the list is reached. |

### Side Effects

|                                    |                                         |
| :--------------------------------- | :-------------------------------------- |
| `Assign(&variable, value)` | Assign `value` to variable. |
| `DeleteArg<N>()` | Delete the `N`-th (0-based) argument, which must be a pointer. |
| `SaveArg<N>(pointer)` | Save the `N`-th (0-based) argument to `*pointer`. |
| `SaveArgPointee<N>(pointer)` | Save the value pointed to by the `N`-th (0-based) argument to `*pointer`. |
| `SetArgReferee<N>(value)` | Assign `value` to the variable referenced by the `N`-th (0-based) argument. |
| `SetArgPointee<N>(value)` | Assign `value` to the variable pointed by the `N`-th (0-based) argument. |
| `SetArgumentPointee<N>(value)` | Same as `SetArgPointee<N>(value)`. Deprecated. Will be removed in v1.7.0. |
| `SetArrayArgument<N>(first, last)` | Copies the elements in source range [`first`, `last`) to the array pointed to by the `N`-th (0-based) argument, which can be either a pointer or an iterator. The action does not take ownership of the elements in the source range. |
| `SetErrnoAndReturn(error, value)` | Set `errno` to `error` and return `value`. |
| `Throw(exception)` | Throws the given exception, which can be any copyable value. Available since v1.1.0. |

### Using a Function, Functor, or Lambda as an Action

In the following, by "callable" we mean a free function, `std::function`,
functor, or lambda.

|                                     |                                        |
| :---------------------------------- | :------------------------------------- |
| `f` | Invoke f with the arguments passed to the mock function, where f is a callable. |
| `Invoke(f)` | Invoke `f` with the arguments passed to the mock function, where `f` can be a global/static function or a functor. |
| `Invoke(object_pointer, &class::method)` | Invoke the method on the object with the arguments passed to the mock function. |
| `InvokeWithoutArgs(f)` | Invoke `f`, which can be a global/static function or a functor. `f` must take no arguments. |
| `InvokeWithoutArgs(object_pointer, &class::method)` | Invoke the method on the object, which takes no arguments. |
| `InvokeArgument<N>(arg1, arg2, ..., argk)` | Invoke the mock function's `N`-th (0-based) argument, which must be a function or a functor, with the `k` arguments. |

The return value of the invoked function is used as the return value of the
action.

When defining a callable to be used with `Invoke*()`, you can declare any unused
parameters as `Unused`:

```cpp
using ::testing::Invoke;
double Distance(Unused, double x, double y) { return sqrt(x*x + y*y); }
...
EXPECT_CALL(mock, Foo("Hi", _, _)).WillOnce(Invoke(Distance));
```

`Invoke(callback)` and `InvokeWithoutArgs(callback)` take ownership of
`callback`, which must be permanent. The type of `callback` must be a base
callback type instead of a derived one, e.g.

```cpp
  BlockingClosure* done = new BlockingClosure;
  ... Invoke(done) ...;  // This won't compile!

  Closure* done2 = new BlockingClosure;
  ... Invoke(done2) ...;  // This works.
```

In `InvokeArgument<N>(...)`, if an argument needs to be passed by reference,
wrap it inside `std::ref()`. For example,

```cpp
using ::testing::InvokeArgument;
...
InvokeArgument<2>(5, string("Hi"), std::ref(foo))
```

calls the mock function's #2 argument, passing to it `5` and `string("Hi")` by
value, and `foo` by reference.

### Default Action

| Matcher       | Description                                            |
| :------------ | :----------------------------------------------------- |
| `DoDefault()` | Do the default action (specified by `ON_CALL()` or the built-in one). |

{: .callout .note}
**Note:** due to technical reasons, `DoDefault()` cannot be used inside a
composite action - trying to do so will result in a run-time error.

### Composite Actions

|                                |                                             |
| :----------------------------- | :------------------------------------------ |
| `DoAll(a1, a2, ..., an)`       | Do all actions `a1` to `an` and return the result of `an` in each invocation. The first `n - 1` sub-actions must return void and will receive a  readonly view of the arguments. |
| `IgnoreResult(a)`              | Perform action `a` and ignore its result. `a` must not return void. |
| `WithArg<N>(a)`                | Pass the `N`-th (0-based) argument of the mock function to action `a` and perform it. |
| `WithArgs<N1, N2, ..., Nk>(a)` | Pass the selected (0-based) arguments of the mock function to action `a` and perform it. |
| `WithoutArgs(a)`               | Perform action `a` without any arguments. |

### Defining Actions

|                                    |                                         |
| :--------------------------------- | :-------------------------------------- |
| `ACTION(Sum) { return arg0 + arg1; }` | Defines an action `Sum()` to return the sum of the mock function's argument #0 and #1. |
| `ACTION_P(Plus, n) { return arg0 + n; }` | Defines an action `Plus(n)` to return the sum of the mock function's argument #0 and `n`. |
| `ACTION_Pk(Foo, p1, ..., pk) { statements; }` | Defines a parameterized action `Foo(p1, ..., pk)` to execute the given `statements`. |

The `ACTION*` macros cannot be used inside a function or class.

## Cardinalities {#CardinalityList}

These are used in `Times()` to specify how many times a mock function will be
called:

|                   |                                                        |
| :---------------- | :----------------------------------------------------- |
| `AnyNumber()`     | The function can be called any number of times.        |
| `AtLeast(n)`      | The call is expected at least `n` times.               |
| `AtMost(n)`       | The call is expected at most `n` times.                |
| `Between(m, n)`   | The call is expected between `m` and `n` (inclusive) times. |
| `Exactly(n) or n` | The call is expected exactly `n` times. In particular, the call should never happen when `n` is 0. |

## Expectation Order

By default, the expectations can be matched in *any* order. If some or all
expectations must be matched in a given order, there are two ways to specify it.
They can be used either independently or together.

### The After Clause {#AfterClause}

```cpp
using ::testing::Expectation;
...
Expectation init_x = EXPECT_CALL(foo, InitX());
Expectation init_y = EXPECT_CALL(foo, InitY());
EXPECT_CALL(foo, Bar())
     .After(init_x, init_y);
```

says that `Bar()` can be called only after both `InitX()` and `InitY()` have
been called.

If you don't know how many pre-requisites an expectation has when you write it,
you can use an `ExpectationSet` to collect them:

```cpp
using ::testing::ExpectationSet;
...
ExpectationSet all_inits;
for (int i = 0; i < element_count; i++) {
  all_inits += EXPECT_CALL(foo, InitElement(i));
}
EXPECT_CALL(foo, Bar())
     .After(all_inits);
```

says that `Bar()` can be called only after all elements have been initialized
(but we don't care about which elements get initialized before the others).

Modifying an `ExpectationSet` after using it in an `.After()` doesn't affect the
meaning of the `.After()`.

### Sequences {#UsingSequences}

When you have a long chain of sequential expectations, it's easier to specify
the order using **sequences**, which don't require you to give each expectation
in the chain a different name. *All expected calls* in the same sequence must
occur in the order they are specified.

```cpp
using ::testing::Return;
using ::testing::Sequence;
Sequence s1, s2;
...
EXPECT_CALL(foo, Reset())
    .InSequence(s1, s2)
    .WillOnce(Return(true));
EXPECT_CALL(foo, GetSize())
    .InSequence(s1)
    .WillOnce(Return(1));
EXPECT_CALL(foo, Describe(A<const char*>()))
    .InSequence(s2)
    .WillOnce(Return("dummy"));
```

says that `Reset()` must be called before *both* `GetSize()` *and* `Describe()`,
and the latter two can occur in any order.

To put many expectations in a sequence conveniently:

```cpp
using ::testing::InSequence;
{
  InSequence seq;

  EXPECT_CALL(...)...;
  EXPECT_CALL(...)...;
  ...
  EXPECT_CALL(...)...;
}
```

says that all expected calls in the scope of `seq` must occur in strict order.
The name `seq` is irrelevant.

## Verifying and Resetting a Mock

gMock will verify the expectations on a mock object when it is destructed, or
you can do it earlier:

```cpp
using ::testing::Mock;
...
// Verifies and removes the expectations on mock_obj;
// returns true if and only if successful.
Mock::VerifyAndClearExpectations(&mock_obj);
...
// Verifies and removes the expectations on mock_obj;
// also removes the default actions set by ON_CALL();
// returns true if and only if successful.
Mock::VerifyAndClear(&mock_obj);
```

You can also tell gMock that a mock object can be leaked and doesn't need to be
verified:

```cpp
Mock::AllowLeak(&mock_obj);
```

## Mock Classes

gMock defines a convenient mock class template

```cpp
class MockFunction<R(A1, ..., An)> {
 public:
  MOCK_METHOD(R, Call, (A1, ..., An));
};
```

See this [recipe](gmock_cook_book.md#using-check-points) for one application of
it.

## Flags

| Flag                           | Description                               |
| :----------------------------- | :---------------------------------------- |
| `--gmock_catch_leaked_mocks=0` | Don't report leaked mock objects as failures. |
| `--gmock_verbose=LEVEL` | Sets the default verbosity level (`info`, `warning`, or `error`) of Google Mock messages. |
