// Following the excellent post by Kyle Kingsbury:
// https://aphyr.com/posts/342-typing-the-technical-interview

struct Nil;

template<typename X, typename Xs>
struct Cons {
    using Head = X;
    using Tail = Xs;
};

template<typename List>
struct FirstWrap { using Res = typename List::Head; };
template<>
struct FirstWrap<Nil> { using Res = Nil; };
template<typename List>
using First = typename FirstWrap<List>::Res;

template<typename List>
using Next = typename List::Tail;

template<typename L1, typename L2>
struct ListConcatWrap {
    using Res = Cons<typename L1::Head,
                     typename ListConcatWrap<typename L1::Tail, L2>::Res>;
};
template<typename L2>
struct ListConcatWrap<Nil, L2> {
    using Res = L2;
};
template<typename L1, typename L2>
using ListConcat = typename ListConcatWrap<L1, L2>::Res;


template<typename L>
struct ListConcatAllWrap {
    using Res = ListConcat<First<L>, typename ListConcatAllWrap<Next<L>>::Res>;
};
template<>
struct ListConcatAllWrap<Nil> { using Res = Nil; };
template<typename L>
using ListConcatAll = typename ListConcatAllWrap<L>::Res;

struct True;
struct False;

template<typename B>
struct NotWrap;
template<>
struct NotWrap<True> { using Res = False; };
template<>
struct NotWrap<False> { using Res = True; };
template<typename B>
using Not = typename NotWrap<B>::Res;

template<typename B1, typename B2>
struct OrWrap { using Res = False; };
template<typename B>
struct OrWrap<True, B> { using Res = True; };
template<typename B>
struct OrWrap<B, True> { using Res = True; };
template<>
struct OrWrap<True, True> { using Res = True; };
template<typename B1, typename B2>
using Or = typename OrWrap<B1, B2>::Res;

template<typename L>
struct AnyTrueWrap;
template<typename L>
struct AnyTrueWrap<Cons<True, L>> { using Res = True; };
template<typename L>
struct AnyTrueWrap<Cons<False, L>> { using Res = typename AnyTrueWrap<L>::Res; };
template<>
struct AnyTrueWrap<Nil> { using Res = False; };
template<typename L>
using AnyTrue = typename AnyTrueWrap<L>::Res;

struct Z;
template<typename N>
struct S { using P = N; };

using N0 = Z;
using N1 = S<N0>;
using N2 = S<N1>;
using N3 = S<N2>;
using N4 = S<N3>;
using N5 = S<N4>;
using N6 = S<N5>;
using N7 = S<N6>;
using N8 = S<N7>;

template<typename A, typename B>
struct PeanoEqualWrap { 
    using Res = typename PeanoEqualWrap<typename A::P, typename B::P>::Res;
};
template<typename A>
struct PeanoEqualWrap<S<A>, Z> { using Res = False; };
template<typename A>
struct PeanoEqualWrap<Z, S<A>> { using Res = False; };
template<>
struct PeanoEqualWrap<Z, Z> { using Res = True; };
template<typename A, typename B>
using PeanoEqual = typename PeanoEqualWrap<A, B>::Res;

template<typename A, typename B>
struct PeanoLTWrap { 
    using Res = typename PeanoLTWrap<typename A::P, typename B::P>::Res;
};
template<typename A>
struct PeanoLTWrap<S<A>, Z> { using Res = False; };
template<typename A>
struct PeanoLTWrap<Z, S<A>> { using Res = True; };
template<>
struct PeanoLTWrap<Z, Z> { using Res = False; };
template<typename A, typename B>
using PeanoLT = typename PeanoLTWrap<A, B>::Res;

template<typename A, typename B>
struct PeanoAbsDiffWrap { 
    using Res = typename PeanoAbsDiffWrap<typename A::P, typename B::P>::Res;
};
template<typename A>
struct PeanoAbsDiffWrap<S<A>, Z> { using Res = S<A>; };
template<typename A>
struct PeanoAbsDiffWrap<Z, S<A>> { using Res = S<A>; };
template<>
struct PeanoAbsDiffWrap<Z, Z> { using Res = Z; };
template<typename A, typename B>
using PeanoAbsDiff = typename PeanoAbsDiffWrap<A, B>::Res;

template <typename N>
struct RangeWrap;
template <typename N>
struct RangeWrap<S<N>> { using Res = Cons<N, typename RangeWrap<N>::Res>; };
template <>
struct RangeWrap<Z> { using Res = Nil; };
template <typename N>
using Range = typename RangeWrap<N>::Res;

template<template <typename> class Fn, typename Arg>
struct ApplyWrap {
    using Res = Fn<Arg>;
};
template<template <typename> class Fn, typename Arg>
using Apply = typename ApplyWrap<Fn, Arg>::Res;

template<template <typename> class Fn, typename L>
struct MapWrap {
    using Res = Cons<Apply<Fn, First<L>>, typename MapWrap<Fn, Next<L>>::Res>;
};
template<template <typename> class Fn>
struct MapWrap<Fn, Nil> { using Res = Nil; };
template<template <typename> class Fn, typename L>
using Map = typename MapWrap<Fn, L>::Res;

template<template <typename> class Fn, typename L>
struct MapCatWrap {
    using Res = ListConcatAll<Map<Fn, L>>;
};
template<template <typename> class Fn>
struct MapCatWrap<Fn, Nil> { using Res = Nil; };
template<template <typename> class Fn, typename L>
using MapCat = typename MapCatWrap<Fn, L>::Res;

template<typename Pred, typename El, typename L>
struct AppendIfWrap;
template<typename El, typename L>
struct AppendIfWrap<True, El, L> { using Res = Cons<El, L>; };
template<typename El, typename L>
struct AppendIfWrap<False, El, L> { using Res = L; };
template<typename Pred, typename El, typename L>
using AppendIf = typename AppendIfWrap<Pred, El, L>::Res;

template<template <typename> class Fn, typename L>
struct FilterWrap {
    using Res = AppendIf<Apply<Fn, First<L>>,
                         First<L>,
                         typename FilterWrap<Fn, Next<L>>::Res>;
};
template<template <typename> class Fn>
struct FilterWrap<Fn, Nil> { using Res = Nil; };
template<template <typename> class Fn, typename L>
using Filter = typename FilterWrap<Fn, L>::Res;

template<typename X_, typename Y_>
struct Queen { using X = X_; using Y = Y_; };

// Couldn't figure out a way to have Queen1Wrap<X> { template<Y> .. }
// and then an using alias for that partial template. So instead inline all the partial
// aplication structs into where they are used and use "::template T" to refer to the member.

template<typename N, typename X>
struct QueensInRowWrap {
    struct Queen1 {
        template <typename Y>
        using T = Queen<X, Y>;
    };
    using Res = Map<Queen1::template T, Range<N>>;
};
template<typename N, typename X>
using QueensInRow = typename QueensInRowWrap<N, X>::Res;

template<typename Qa, typename Qb>
using Threatens =
    Or<
        Or<PeanoEqual<typename Qa::X, typename Qb::X>,
           PeanoEqual<typename Qa::Y, typename Qb::Y>>,
        PeanoEqual<PeanoAbsDiff<typename Qa::X, typename Qb::X>,
                   PeanoAbsDiff<typename Qa::Y, typename Qb::Y>>>;

template <typename Configs, typename Q>
struct SafeWrap {
    struct Threatens1 {
        template <typename QOther>
        using T = Threatens<Q, QOther>;
    };
    using Res = Not<AnyTrue<Map<Threatens1::template T, Configs>>>;
};
template <typename Configs, typename Q>
using Safe = typename SafeWrap<Configs, Q>::Res;

template<typename N, typename X, typename Config>
struct AddQueenWrap {
    struct Safe1 {
        template <typename Q>
        using T = Safe<Config, Q>;
    };
    struct Conj1 {
        template <typename El>
        using T = Cons<El, Config>;
    };
    using Res = Map<Conj1::template T, Filter<Safe1::template T, QueensInRow<N, X>>>;
};
template<typename N, typename X, typename Config>
using AddQueen = typename AddQueenWrap<N, X, Config>::Res;

template<typename N, typename X, typename Configs>
struct AddQueenToAllWrap {
    struct AddQueen2 {
        template <typename Config>
        using T = AddQueen<N, X, Config>;
    };
    using Res = MapCat<AddQueen2::template T, Configs>;
};
template<typename N, typename X, typename Configs>
using AddQueenToAll = typename AddQueenToAllWrap<N, X, Configs>::Res;

template<typename Pred, typename N, typename X, typename Config>
struct AddQueensIfWrap;
template<typename Pred, typename N, typename X, typename Config>
using AddQueensIf = typename AddQueensIfWrap<Pred, N, X, Config>::Res;

template<typename N, typename X, typename Config>
using AddQueens = AddQueensIf<PeanoLT<X, N>, N, X, Config>;

template<typename N, typename X, typename Config>
struct AddQueensIfWrap<True, N, X, Config> { 
    using Res = AddQueens<N, S<X>, AddQueenToAll<N, X, Config>>;
};
template<typename N, typename X, typename Config>
struct AddQueensIfWrap<False, N, X, Config> { using Res = Config; };

template<typename N>
using Solution = First<AddQueens<N, N0, Cons<Nil, Nil>>>;

// We only use std::is_same_v for testing & output,
// Otherwise no stdlib! That would make things too easy.
#include <type_traits>

#ifdef NQUEENS
static_assert(std::is_same_v<Solution<NQUEENS>, Nil>);
#endif

// "Unit tests" 
namespace tests {
    struct A;
    struct B;
    struct C;
    namespace lists {
        using LA = Cons<A, Nil>;
        using LB = Cons<B, LA>;
        using LC = Cons<C, LB>;
        static_assert(std::is_same_v<First<LA>, A>);
        static_assert(std::is_same_v<First<LB>, B>);
        static_assert(std::is_same_v<First<LC>, C>);

        static_assert(std::is_same_v<First<Next<LA>>, Nil>);
        static_assert(std::is_same_v<First<Next<LB>>, A>);
        static_assert(std::is_same_v<First<Next<LC>>, B>);
        
        using LBC = Cons<B, Cons<C, Nil>>;
        using LABC = ListConcat<LA, LBC>;
        static_assert(std::is_same_v<First<LABC>, A>);
        static_assert(std::is_same_v<First<Next<LABC>>, B>);
        static_assert(std::is_same_v<First<Next<Next<LABC>>>, C>);
        static_assert(std::is_same_v<First<Next<Next<Next<LABC>>>>, Nil>);
        using LBCA = ListConcat<LBC, LA>;
        static_assert(std::is_same_v<First<LBCA>, B>);
        static_assert(std::is_same_v<First<Next<LBCA>>, C>);
        static_assert(std::is_same_v<First<Next<Next<LBCA>>>, A>);
        static_assert(std::is_same_v<First<Next<Next<Next<LBCA>>>>, Nil>);

        using LAB = Cons<A, Cons<B, Nil>>;
        // X = ( (A) (A B) (B C) )
        using X = Cons<LA, Cons<LAB, Cons<LBC, Nil>>>;
        static_assert(std::is_same_v<First<First<X>>, A>);
        static_assert(std::is_same_v<Next<First<X>>, Nil>);
        static_assert(std::is_same_v<First<First<Next<X>>>, A>);
        static_assert(std::is_same_v<First<Next<First<Next<X>>>>, B>);
        // Y = (A A B B C)
        using Y = ListConcatAll<X>;
        static_assert(std::is_same_v<First<Y>, A>);
        static_assert(std::is_same_v<First<Next<Y>>, A>);
        static_assert(std::is_same_v<First<Next<Next<Y>>>, B>);
        static_assert(std::is_same_v<First<Next<Next<Next<Y>>>>, B>);
        static_assert(std::is_same_v<First<Next<Next<Next<Next<Y>>>>>, C>);
        static_assert(std::is_same_v<First<Next<Next<Next<Next<Next<Y>>>>>>, Nil>);

        using LRangeOne = Cons<N0, Nil>;
        using LRangeTwo = Cons<N1, LRangeOne>;
        static_assert(std::is_same_v<Range<N0>, Nil>);
        static_assert(std::is_same_v<Range<N1>, LRangeOne>);
        static_assert(std::is_same_v<Range<N2>, LRangeTwo>);
    }
    namespace bools {
        static_assert(std::is_same_v<Not<True>, False>);
        static_assert(std::is_same_v<Not<False>, True>);
        
        static_assert(std::is_same_v<Or<False, False>, False>);
        static_assert(std::is_same_v<Or<True, False>, True>);
        static_assert(std::is_same_v<Or<False, True>, True>);
        static_assert(std::is_same_v<Or<True, True>, True>);
    
        using LT = Cons<True, Nil>;
        using LF = Cons<False, Nil>;
        using LFT = Cons<False, Cons<True, Nil>>;
        static_assert(std::is_same_v<AnyTrue<Nil>, False>);
        static_assert(std::is_same_v<AnyTrue<LT>, True>);
        static_assert(std::is_same_v<AnyTrue<LF>, False>);
        static_assert(std::is_same_v<AnyTrue<LFT>, True>);
    }
    namespace peano {
        static_assert(std::is_same_v<PeanoEqual<N0, N0>, True>);
        static_assert(std::is_same_v<PeanoEqual<N1, N1>, True>);
        static_assert(std::is_same_v<PeanoEqual<N1, N2>, False>);
        static_assert(std::is_same_v<PeanoEqual<N8, N8>, True>);
        static_assert(std::is_same_v<PeanoEqual<N6, N7>, False>);

        static_assert(std::is_same_v<PeanoLT<N0, N0>, False>);
        static_assert(std::is_same_v<PeanoLT<N0, N1>, True>);
        static_assert(std::is_same_v<PeanoLT<N0, N2>, True>);
        static_assert(std::is_same_v<PeanoLT<N1, N2>, True>);
        static_assert(std::is_same_v<PeanoLT<N2, N2>, False>);
        
        static_assert(std::is_same_v<PeanoAbsDiff<N0, N0>, N0>);
        static_assert(std::is_same_v<PeanoAbsDiff<N1, N1>, N0>);
        static_assert(std::is_same_v<PeanoAbsDiff<N0, N8>, N8>);
        static_assert(std::is_same_v<PeanoAbsDiff<N8, N0>, N8>);
        static_assert(std::is_same_v<PeanoAbsDiff<N3, N7>, N4>);
    }
    namespace functional {
        static_assert(std::is_same_v<Apply<S, N0>, N1>);
        static_assert(std::is_same_v<Apply<Not, False>, True>);

        using LTF = Cons<True, Cons<False, Nil>>;
        using LFT = Cons<False, Cons<True, Nil>>;
        static_assert(std::is_same_v<Map<Not, LTF>, LFT>);
        
        using LEven = Cons<N0, Cons<N2, Cons<N4, Cons<N6, Nil>>>>;
        using LOdd = Cons<N1, Cons<N3, Cons<N5, Cons<N7, Nil>>>>;
        static_assert(std::is_same_v<Map<S, LEven>, LOdd>);

        template <typename A>
        using PrependOne = Cons<N1, Cons<A, Nil>>;
        static_assert(std::is_same_v<MapCat<PrependOne, LTF>,
                                     Cons<N1, Cons<True, Cons<N1, Cons<False, Nil>>>>>);

        using LNums = Cons<N0, Cons<N1, Cons<N2, Cons<N3, Cons<N4, Nil>>>>>;
        using LNumsBelowThree = Cons<N0, Cons<N1, Cons<N2, Nil>>>;
        template <typename A>
        using LessThanThree = PeanoLT<A, N3>;
        static_assert(std::is_same_v<Filter<LessThanThree, LNums>, LNumsBelowThree>);
        
        using LB = Cons<B, Nil>;
        using LAB = Cons<A, LB>;
        static_assert(std::is_same_v<AppendIf<True, A, LB>, LAB>);
        static_assert(std::is_same_v<AppendIf<False, A, LB>, LB>);
    }
    namespace queens {
        static_assert(std::is_same_v<QueensInRow<N2, N7>, Cons<Queen<N7, N1>, Cons<Queen<N7, N0>, Nil>>>);

        static_assert(std::is_same_v<Threatens<Queen<N0, N0>, Queen<N0, N1>>, True>);
        static_assert(std::is_same_v<Threatens<Queen<N0, N0>, Queen<N4, N0>>, True>);
        static_assert(std::is_same_v<Threatens<Queen<N0, N0>, Queen<N3, N3>>, True>);
        static_assert(std::is_same_v<Threatens<Queen<N0, N0>, Queen<N3, N5>>, False>);
        static_assert(std::is_same_v<Threatens<Queen<N0, N0>, Queen<N1, N2>>, False>);

        using SmallConfig = Cons<Queen<N0, N0>, Cons<Queen<N1, N2>, Nil>>;
        static_assert(std::is_same_v<Safe<SmallConfig, Queen<N1, N1>>, False>);
        static_assert(std::is_same_v<Safe<SmallConfig, Queen<N2, N2>>, False>);
        static_assert(std::is_same_v<Safe<SmallConfig, Queen<N3, N1>>, True>);
        static_assert(std::is_same_v<Safe<SmallConfig, Queen<N2, N4>>, True>);

        static_assert(std::is_same_v<AddQueen<N1, N0, Nil>,
                                     Cons<Cons<Queen<N0, N0>, Nil>, Nil>>);
        static_assert(std::is_same_v<AddQueen<N1, N0, Cons<Queen<N0, N0>, Nil>>, Nil>);
        static_assert(std::is_same_v<AddQueen<N3, N0, SmallConfig>, Nil>);
        static_assert(std::is_same_v<AddQueen<N3, N1, SmallConfig>, Nil>);
        static_assert(std::is_same_v<AddQueen<N3, N2, SmallConfig>, Nil>);
        static_assert(std::is_same_v<AddQueen<N3, N3, SmallConfig>,
                                     Cons<Cons<Queen<N3, N1>, SmallConfig>, Nil>>);
        
        static_assert(std::is_same_v<AddQueenToAll<N4, N1, Cons<Nil, Nil>>,
                                     Cons<Cons<Queen<N1, N3>, Nil>,
                                     Cons<Cons<Queen<N1, N2>, Nil>,
                                     Cons<Cons<Queen<N1, N1>, Nil>,
                                     Cons<Cons<Queen<N1, N0>, Nil>, Nil>>>>>);
        static_assert(std::is_same_v<AddQueenToAll<N4, N2, Cons<SmallConfig, Nil>>, Nil>);
        static_assert(std::is_same_v<AddQueenToAll<N4, N3, Cons<SmallConfig, Nil>>,
                                     Cons<Cons<Queen<N3, N1>, SmallConfig>, Nil>>);
        using OtherConfig = Cons<Queen<N0, N2>, Cons<Queen<N1, N0>, Nil>>; 
        static_assert(std::is_same_v<AddQueenToAll<N4, N3, Cons<OtherConfig, Cons<SmallConfig, Nil>>>,
                                     Cons<Cons<Queen<N3, N3>, OtherConfig>,
                                     Cons<Cons<Queen<N3, N1>, OtherConfig>, 
                                     Cons<Cons<Queen<N3, N1>, SmallConfig>, Nil>>>>);

        static_assert(std::is_same_v<AddQueens<N1, N0, Cons<Nil, Nil>>, 
                                     Cons<Cons<Queen<N0, N0>, Nil>, Nil>>);
    }
}