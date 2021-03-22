class CountSort{
public:
    static long long CounterCmp;
    static long long CounterSwap;
};

template<typename CmpType>
bool CmpWithCount(CmpType a, CmpType b) {
    CountSort::CounterCmp += 1;            // Counter on number of comparing in sortings
    return a > b;
}

template<typename CmpType> 
bool DefCmp(CmpType a, CmpType b) {
    return a > b;
}

template<typename SwapType> 
void Swap(SwapType* a, SwapType* b) {
    SwapType tmp = *a;
    *a = *b;
    *b = tmp;
    CountSort::CounterSwap += 1;           // Counter on number of swapping in sortings 
    return;
}

template<typename SortingType> 
void BubbleSort(SortingType* Begin, SortingType* End, bool (*cmp)(SortingType, SortingType) = DefCmp<SortingType>) {

    for (SortingType* Base = Begin; Base != End; ++Base) {
        for (SortingType* MinSearch = Base + 1; MinSearch != End + 1; ++MinSearch) {
            if (cmp(*Base, *MinSearch)) {
                Swap(Base, MinSearch);
            }
        }
    }

    return;
}

template<typename SortingType>
void QuickSort(SortingType* Begin, SortingType* End, bool (*cmp)(SortingType, SortingType) = DefCmp<SortingType>) {
    SortingType* Left = Begin;
    SortingType* Right = End;
    SortingType Pivot = *Left;

    while (Left < Right) {
        while (!cmp(Pivot, *Right) && (Left < Right))
            --Right;
        if (Left != Right) {
            Swap(Left, Right);
            ++Left;
        }
        while (!cmp(*Left, Pivot) && (Left < Right))
            ++Left;
        if (Right != Left) {
            Swap(Left, Right);
            --Right;
        }
    }
    *Left = Pivot;
    if (Begin < Left) {
        QuickSort(Begin, Left, cmp);
    }
    if (End > Left) {
        QuickSort(Left + 1, End, cmp);
    }
    return;
}

template<typename SortingType>
void Merge(SortingType* Begin, SortingType* End, SortingType* Middle, bool (*cmp)(SortingType, SortingType) = DefCmp<SortingType>) {
    SortingType* First  = Begin;
    SortingType* Second = Middle + 1;
    const size_t TmpBuffSize = End - Begin + 1;
    SortingType* TmpArr = new SortingType[TmpBuffSize];
    int Counter = 0;

    while (First <= Middle && Second <= End) {
        if (cmp(*Second, *First)) {
            *(TmpArr + Counter) = *First;
            ++Counter;
            ++First;
        }
        else {
            *(TmpArr + Counter) = *Second;
            ++Counter;
            ++Second;
            CountSort::CounterSwap += Middle - First + 1;           // Counter on number of swapping in sortings 
        }
    }
    
    while (First <= Middle) {
        *(TmpArr + Counter) = *First;
        ++Counter;
        ++First;
        CountSort::CounterSwap += Middle - First + 1;           // Counter on number of swapping in sortings 
    }
    while (Second <= End) { 
        *(TmpArr + Counter) = *Second;
        ++Counter;
        ++Second;
    }

    for (int i = 0; i < Counter; ++i) {
        *Begin = *(TmpArr + i);
        ++Begin;
    }
    delete[] TmpArr;
    return;
}

template<typename SortingType>
void MergeSort(SortingType* Begin, SortingType* End, bool (*cmp)(SortingType, SortingType) = DefCmp<SortingType>) {
    SortingType* MiddleFlag = 0;
        
    if (Begin < End) {
        MiddleFlag = Begin + (End - Begin) / 2;

        MergeSort(Begin, MiddleFlag, cmp);
        MergeSort(MiddleFlag + 1, End, cmp);
        Merge(Begin, End, MiddleFlag, cmp);
    }

    return;
}
