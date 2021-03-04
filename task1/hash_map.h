#include <bits/stdc++.h>

template<class KeyType, class ValueType, class Hash = std::hash<KeyType> >
class HashMap {
private:
    size_t SZ;
    size_t cap;
    const size_t A = 2;
    Hash hasher;
    std::vector<std::list<std::pair<const KeyType, ValueType> > > table;
    size_t begin_ind;
    size_t end_ind;
    void rebuid();

public:

    class iterator {
    private:
        typename std::list<std::pair<const KeyType, ValueType> >::iterator it;
        size_t ind;
        HashMap* ptr;
    public:
        iterator() = default;
        iterator(typename std::list<std::pair<const KeyType, ValueType> >::iterator nit, size_t nind, HashMap* pr) :
        it(nit), ind(nind), ptr(pr) {
        }
        bool operator==(iterator oth) const {
            return it == oth.it;
        }
        bool operator!=(iterator oth) const {
            return it != oth.it;
        }
        iterator& operator++() {
            ++it;
            if (ind == ptr->end_ind && it == ptr->table[ind].end()) {
                return *this;
            }
            while (ind < ptr->end_ind && it == ptr->table[ind].end()) {
                ++ind;
                it = ptr->table[ind].begin();
            }
            return *this;
        }
        iterator operator++(int) {
            auto temp = *this;
            ++(*this);
            return temp;
        }
        std::pair<const KeyType, ValueType>& operator*() {
            return *it;
        }
        std::pair<const KeyType, ValueType>* operator->() {
            return &(*it);
        }
    };

    class const_iterator {
    private:
        typename std::list<std::pair<const KeyType, ValueType> >::const_iterator it;
        size_t ind;
        const HashMap* ptr;
    public:
        const_iterator(typename std::list<std::pair<const KeyType, ValueType> >::const_iterator nit, size_t nind, const HashMap* pr) : it(nit), ind(nind), ptr(pr) {
        }
        const_iterator() = default;
        bool operator==(const_iterator oth) const {
            return it == oth.it;
        }
        bool operator!=(const_iterator oth) const {
            return it != oth.it;
        }
        const_iterator& operator++() {
            ++it;
            if (ind == ptr->end_ind && it == ptr->table[ind].end()) {
                return *this;
            }
            while (ind < ptr->end_ind && it == ptr->table[ind].end()) {
                ++ind;
                it = ptr->table[ind].begin();
            }
            return *this;
        }
        const_iterator operator++(int) {
            auto temp = *this;
            ++(*this);
            return temp;
        }
        const std::pair<const KeyType, ValueType>& operator*() const {
            return *it;
        }
        const std::pair<const KeyType, ValueType>* operator->() const {
            return &(*it);
        }
    };

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    
    HashMap(Hash h = Hash());
    template<class InputIt>
    HashMap(InputIt begin, InputIt end, Hash h = Hash());
    HashMap(std::initializer_list<std::pair<const KeyType, ValueType> > l, Hash h = Hash());
    
    void insert(const std::pair<const KeyType, ValueType>& x);
    void erase(const KeyType& x);
    iterator find(const KeyType& x);
    const_iterator find(const KeyType& x) const;
    void clear();
    size_t size() const;
    bool empty() const;
    Hash hash_function() const;
    const ValueType& at(const KeyType& x) const;

    ValueType& operator[](const KeyType& x);
    HashMap<KeyType, ValueType>& operator=(const HashMap<KeyType, ValueType>& oth);
};

template<class KeyType, class ValueType, class Hash>
void HashMap<KeyType, ValueType, Hash>::rebuid() {
    std::vector<std::pair<const KeyType, ValueType> > temp;
    for (auto it = begin(); it != end(); ++it) {
        temp.push_back(*it);
    }
    size_t ncap = cap * 4;
    clear();
    cap = ncap;
    table.resize(ncap);
    for (auto& i : temp) {
        insert(i);
    }
}

template<class KeyType, class ValueType, class Hash>
void HashMap<KeyType, ValueType, Hash>::clear() {
    while (begin() != end()) {
        erase(begin()->first);
    }

    begin_ind = end_ind = 0;
    cap = 8;
    table.resize(cap);
}

template<class KeyType, class ValueType, class Hash>
HashMap<KeyType, ValueType, Hash>::HashMap(Hash h) : hasher(h) {
    SZ = 0;
    cap = 8;
    table.resize(cap);
    begin_ind = end_ind = 0;
}

template<class KeyType, class ValueType, class Hash>
template<class InputIt>
HashMap<KeyType, ValueType, Hash>::HashMap(InputIt begin, InputIt end, Hash h) : hasher(h) {
    SZ = 0;
    cap = 8;
    table.resize(cap);
    begin_ind = end_ind = 0;
    while (begin != end) {
        insert(*begin);
        ++begin;
    }
}

template<class KeyType, class ValueType, class Hash>
HashMap<KeyType, ValueType, Hash>::HashMap(std::initializer_list<std::pair<const KeyType, ValueType> > l, Hash h) : hasher(h) {
    SZ = 0;
    cap = 8;
    table.resize(cap);
    begin_ind = end_ind = 0;
    for (auto it = l.begin(); it != l.end(); ++it) {
        insert(*it);
    }
}

template<class KeyType, class ValueType, class Hash>
void HashMap<KeyType, ValueType, Hash>::insert(const std::pair<const KeyType, ValueType>& x) {
    size_t hs = hasher(x.first) % cap;
    for (auto it = table[hs].begin(); it != table[hs].end(); ++it) {
        if (it->first == x.first) {
            return;
        }
    }
    table[hs].emplace_back(x);
    SZ++;
    begin_ind = std::min(begin_ind, hs);
    end_ind = std::max(end_ind, hs);
    if (SZ == 1) {
        begin_ind = hs;
        end_ind = hs;
    }
    if (SZ * A > cap) {
        rebuid();
    }
}

template<class KeyType, class ValueType, class Hash>
size_t HashMap<KeyType, ValueType, Hash>::size() const {
    return SZ;
}

template<class KeyType, class ValueType, class Hash>
bool HashMap<KeyType, ValueType, Hash>::empty() const {
    return size() == 0;
}

template<class KeyType, class ValueType, class Hash>
Hash HashMap<KeyType, ValueType, Hash>::hash_function() const {
    return hasher;
}

template<class KeyType, class ValueType, class Hash>
typename HashMap<KeyType, ValueType, Hash>::iterator HashMap<KeyType, ValueType, Hash>::begin() {
    return iterator(table[begin_ind].begin(), begin_ind, this);
}
template<class KeyType, class ValueType, class Hash>
typename HashMap<KeyType, ValueType, Hash>::iterator HashMap<KeyType, ValueType, Hash>::end() {
    return iterator(table[end_ind].end(), end_ind, this);
}
template<class KeyType, class ValueType, class Hash>
typename HashMap<KeyType, ValueType, Hash>::const_iterator HashMap<KeyType, ValueType, Hash>::begin() const {
    return const_iterator(table[begin_ind].begin(), begin_ind, this);
}
template<class KeyType, class ValueType, class Hash>
typename HashMap<KeyType, ValueType, Hash>::const_iterator HashMap<KeyType, ValueType, Hash>::end() const {
    return const_iterator(table[end_ind].end(), end_ind, this);
}

template<class KeyType, class ValueType, class Hash>
void HashMap<KeyType, ValueType, Hash>::erase(const KeyType& x) {
    size_t hs = hasher(x) % cap;
    auto it = table[hs].end();
    for (auto i = table[hs].begin(); i != table[hs].end(); ++i) {
        if (i->first == x) {
            it = i;
            break;
        }
    }
    if (it == table[hs].end()) {
        return;
    }
    table[hs].erase(it);
    --SZ;
    if (table[hs].empty()) {
        if (end_ind == hs) {
            while(table[end_ind].empty() && end_ind > begin_ind) {
                --end_ind;
            }
        } else if (begin_ind == hs) {
            while (table[begin_ind].empty() && begin_ind < end_ind) {
                ++begin_ind;
            }
        }
    }
}

template<class KeyType, class ValueType, class Hash>
typename HashMap<KeyType, ValueType, Hash>::iterator HashMap<KeyType, ValueType, Hash>::find(const KeyType& x) {
    size_t hs = hasher(x) % cap;
    auto it = table[hs].end();
    for (auto i = table[hs].begin(); i != table[hs].end(); ++i) {
        if (i->first == x) {
            it = i;
            break;
        }
    }
    if (it == table[hs].end()) {
        return end();
    }
    return iterator(it, hs, this);
}

template<class KeyType, class ValueType, class Hash>
typename HashMap<KeyType, ValueType, Hash>::const_iterator HashMap<KeyType, ValueType, Hash>::find(const KeyType& x) const {
    size_t hs = hasher(x) % cap;
    auto it = table[hs].end();
    for (auto i = table[hs].begin(); i != table[hs].end(); ++i) {
        if (i->first == x) {
            it = i;
            break;
        }
    }
    if (it == table[hs].end()) {
        return end();
    }
    return const_iterator(it, hs, this);
}

template<class KeyType, class ValueType, class Hash>
ValueType& HashMap<KeyType, ValueType, Hash>::operator[](const KeyType& x) {
    auto it = find(x);
    if (it == end()) {
        insert({x, ValueType()});
    }
    it = find(x);
    return (*it).second;
}

template<class KeyType, class ValueType, class Hash>
const ValueType& HashMap<KeyType, ValueType, Hash>::at(const KeyType& x) const {
    auto it = find(x);
    if (it == end()) {
        throw std::out_of_range ("Out of range!");
    }
    return (*it).second;
}

template<class KeyType, class ValueType, class Hash>
HashMap<KeyType, ValueType>& HashMap<KeyType, ValueType, Hash>::operator=(const HashMap<KeyType, ValueType>& oth) {
    const HashMap<KeyType, ValueType>* ptr = &oth;
    if (ptr == this) {
        return *this;
    }
    clear();
    SZ = oth.SZ;
    cap = oth.cap;
    hasher = oth.hasher;
    table.resize(cap);
    for (auto i = oth.begin(); i != oth.end(); ++i) {
        insert(*i);
    }
    return *this;
}