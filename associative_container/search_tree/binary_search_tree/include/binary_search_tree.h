#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_BINARY_SEARCH_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_BINARY_SEARCH_TREE_H

#include <list>
#include <stack>
#include <vector>
#include <logger.h>
#include <logger_guardant.h>
#include <allocator.h>
#include <allocator_guardant.h>
#include <not_implemented.h>
#include <search_tree.h>

template<
    typename tkey,
    typename tvalue>
class binary_search_tree:
    public search_tree<tkey, tvalue>
{

protected:
    
    struct node
    {
    
    public:
        
        tkey key;
        
        tvalue value;
        
        node *left_subtree;
        
        node *right_subtree;
    
    public:
        
        explicit node(
            tkey const &key,
            tvalue const &value);
        
        explicit node(
            tkey const &key,
            tvalue &&value);
        
    };


public:
    unsigned int get_depth(node const * test_node) const;
    static bool is_templ_node(node const * test_node);

    class prefix_iterator;
    class prefix_const_iterator;
    class prefix_reverse_iterator;
    class prefix_reverse_const_iterator;
    class infix_iterator;
    class infix_const_iterator;
    class infix_reverse_iterator;
    class infix_const_reverse_iterator;
    //class infix_reverse_const_iterator;
    class postfix_iterator;
    class postfix_const_iterator;
    class postfix_reverse_iterator;
    class postfix_reverse_const_iterator;
    
    struct iterator_data
    {
        friend class prefix_iterator;
        friend class prefix_const_iterator;
        friend class prefix_reverse_iterator;
        friend class prefix_reverse_const_iterator;
        friend class infix_iterator;
        friend class infix_const_iterator;
        friend class infix_reverse_iterator;
        friend class prefix_const_reverse_iterator;
        friend class infix_const_reverse_iterator;
        //friend class infix_reverse_const_iterator;
        friend class postfix_iterator;
        friend class postfix_const_iterator;
        friend class postfix_reverse_iterator;
        friend class postfix_reverse_const_iterator;

    public:
        
        unsigned int depth;
        
        tkey key;        
        tvalue value;
    
    public:
    
        iterator_data() = default;

        explicit iterator_data(
            unsigned int depth,
            tkey const &key,
            tvalue const &value);        
            
        void set(node const * n, binary_search_tree const * t)
        {
            if (n == nullptr)
                return;

            key = n->key;
            value = n->value;

            if (t != nullptr)
                depth = t->get_depth(n);
        }
    };
    
    class base_iterator
    {
    protected:
        base_iterator(binary_search_tree<tkey, tvalue> const * tree)
            : _tree(tree)
        {
            _data = _tree->create_iterator_data();
        }

        ~base_iterator()
        {
            _tree->deallocate_with_guard(_data);
        }

    public:
        bool operator==(base_iterator const & rh) const noexcept
        {
            return _subtree_root->key == rh._subtree_root->key;
        }

        bool operator!=(base_iterator const & rh) const noexcept
        {
            return _subtree_root->key != rh._subtree_root->key;
        }

        void get_right_leaf()
        {
            if (_subtree_root == nullptr)
                return;

            node_stack.push_back(_subtree_root);

            node *cur_node = _subtree_root;
            while (!is_templ_node(cur_node->right_subtree))
            {
                node_stack.push_back(cur_node);
                cur_node = cur_node->right_subtree;
            }

            _subtree_root = cur_node;
        }

        void get_left_leaf()
        {
            if (_subtree_root == nullptr)
                return;

            node_stack.push_back(_subtree_root);

            node *cur_node = _subtree_root;
            while (!is_templ_node(cur_node->left_subtree))
            {
                cur_node = cur_node->left_subtree;
                node_stack.push_back(cur_node);                
            }

            _subtree_root = cur_node;
        }

    protected:
        binary_search_tree<tkey, tvalue> const * _tree;
        binary_search_tree<tkey, tvalue>::node * _subtree_root;

        std::vector<binary_search_tree<tkey, tvalue>::node *> node_stack;
        
        iterator_data *_data = nullptr;

    public:
        binary_search_tree<tkey, tvalue>::node * get_subtree_root() const { return _subtree_root; }
        void set_subtree_root(binary_search_tree<tkey, tvalue>::node *n) { _subtree_root = n; }

        std::vector<binary_search_tree<tkey, tvalue>::node *> & get_node_stack() { return node_stack; }
    };

    template<typename T>
    class iterator : public base_iterator
    {
    public:
        iterator(binary_search_tree<tkey, tvalue> const * tree) : base_iterator(tree)            
        {
        }

        iterator_data * operator*() const
        {
            this->_data->set(this->_subtree_root, this->_tree);
            return this->_data;
        }

        T const operator++(int not_used)
        {
            T res = this;
            ++this;
            return *res;            
        }

    };

    template<typename T>
    class const_iterator : public base_iterator
    {
    public:
        const_iterator(binary_search_tree<tkey, tvalue> const * tree) : base_iterator(tree)
        {            
        }

        iterator_data const * operator*() const
        {
            this->_data->set(this->_subtree_root, this->_tree);
            return this->_data;
        }

        T const operator++(int not_used)
        {
            T res = this;
            ++this;
            return *res;            
        }
    };

    class prefix_iterator final : public iterator<prefix_iterator>
    {
        // friend class binary_search_tree<tkey, tvalue>;

    public:
        explicit prefix_iterator(
            typename binary_search_tree<tkey, tvalue>::node *subtree_root,
            binary_search_tree<tkey, tvalue> *tree);
    
    public:
                
        prefix_iterator &operator++();        
    };
    
    class prefix_const_iterator final : public const_iterator<prefix_const_iterator>
    {
    public:
        explicit prefix_const_iterator(
            typename binary_search_tree<tkey, tvalue>::node *subtree_root,
            binary_search_tree<tkey, tvalue> const * tree);
    
    public:                
        prefix_const_iterator &operator++();
    };
    
    class prefix_reverse_iterator final : public iterator<prefix_reverse_iterator>
    {
    public:        
        explicit prefix_reverse_iterator(
            typename binary_search_tree<tkey, tvalue>::node *subtree_root,
            binary_search_tree<tkey, tvalue> *tree);
    
    public:                
        prefix_reverse_iterator &operator++();

        void set_node_stack(node *);
        void get_first_node();
    };
    
    class prefix_const_reverse_iterator final : public const_iterator<prefix_const_reverse_iterator>
    {    
    public:
        
        explicit prefix_const_reverse_iterator(
            typename binary_search_tree<tkey, tvalue>::node *subtree_root,
            binary_search_tree<tkey, tvalue> *tree);
    
    public:                
        prefix_const_reverse_iterator &operator++();

        void set_node_stack(node *);
        void get_first_node();
    };
    
    class infix_iterator final : public iterator<infix_iterator>
    {
    public:        
        explicit infix_iterator(
            typename binary_search_tree<tkey, tvalue>::node *subtree_root,
            binary_search_tree<tkey, tvalue> *tree);
    
    public:                
        infix_iterator &operator++();
    };
    
    class infix_const_iterator final : public const_iterator<infix_const_iterator>
    {
    public:        
        explicit infix_const_iterator(
            typename binary_search_tree<tkey, tvalue>::node *subtree_root,
            binary_search_tree<tkey, tvalue> const * tree);
    
    public:
        infix_const_iterator &operator++();

    };
    
    class infix_reverse_iterator final : public iterator<infix_reverse_iterator>
    {
    public:        
        explicit infix_reverse_iterator(
            typename binary_search_tree<tkey, tvalue>::node *subtree_root,
            binary_search_tree<tkey, tvalue> *tree);
    
    public:            
        infix_reverse_iterator &operator++();        
    };
    
    class infix_const_reverse_iterator final : public const_iterator<infix_const_reverse_iterator>
    {
    public:        
        explicit infix_const_reverse_iterator(
            typename binary_search_tree<tkey, tvalue>::node *subtree_root,
            binary_search_tree<tkey, tvalue> *tree);
    
    public:        
        infix_const_reverse_iterator &operator++();                
    };
    
    class postfix_iterator final : public iterator<postfix_iterator>
    {
    public:        
        explicit postfix_iterator(
            typename binary_search_tree<tkey, tvalue>::node *subtree_root,
            binary_search_tree<tkey, tvalue> const *tree);
    
    public:                
        postfix_iterator &operator++();

        void set_node_stack(node *);
        void get_first_node();
    };
    
    class postfix_const_iterator final : public const_iterator<postfix_const_iterator>
    {
    public:        
        explicit postfix_const_iterator(
            typename binary_search_tree<tkey, tvalue>::node *subtree_root,
            binary_search_tree<tkey, tvalue> const * tree);
            
        postfix_const_iterator &operator++();   

        void set_node_stack(node *);    
        void get_first_node(); 
    };
    
    class postfix_reverse_iterator final : public iterator<postfix_reverse_iterator>
    {
    public:        
        explicit postfix_reverse_iterator(
            typename binary_search_tree<tkey, tvalue>::node *subtree_root,
            binary_search_tree<tkey, tvalue> *tree);
    
    public:            
        postfix_reverse_iterator &operator++();            
    };
    
    class postfix_const_reverse_iterator final : public const_iterator<postfix_const_reverse_iterator>
    {
    public:
        
        explicit postfix_const_reverse_iterator(
            typename binary_search_tree<tkey, tvalue>::node *subtree_root,
            binary_search_tree<tkey, tvalue> *tree);
    
    public:                
        postfix_const_reverse_iterator &operator++();        
    };

protected:
    
    enum class insertion_of_existent_key_attempt_strategy
    {
        update_value,
        throw_an_exception
    };
    
    enum class disposal_of_nonexistent_key_attempt_strategy
    {
        do_nothing,
        throw_an_exception
    };
    
    class insertion_of_existent_key_attempt_exception final:
        public std::logic_error
    {
    
    private:
        
        tkey _key;
    
    public:
        
        explicit insertion_of_existent_key_attempt_exception(
            tkey const &key);
        
    public:
        
        tkey const &get_key() const noexcept;
    
    };
    
    class obtaining_of_nonexistent_key_attempt_exception final:
        public std::logic_error
    {
    
    private:
        
        tkey _key;
        
    public:
        
        explicit obtaining_of_nonexistent_key_attempt_exception(
            tkey const &key);
        
    public:
        
        tkey const &get_key() const noexcept;
        
    };
    
    class disposal_of_nonexistent_key_attempt_exception final:
        public std::logic_error
    {
    
    private:
        
        tkey _key;
    
    public:
        
        explicit disposal_of_nonexistent_key_attempt_exception(
            tkey const &key);
        
    public:
        
        tkey const &get_key() const noexcept;
    
    };
    
    class template_method_basics:
        public logger_guardant
    {
    
    protected:
        binary_search_tree<tkey, tvalue> *_tree;
        
    public:
    
        explicit template_method_basics(
            binary_search_tree<tkey, tvalue> *tree);

    protected:
    
        [[nodiscard]] logger *get_logger() const noexcept final;
        
    };

    class insertion_template_method:
        public template_method_basics,
        public allocator_guardant
    {
    
    private:
        
        //binary_search_tree<tkey, tvalue> *_tree;
    
    public:
        
        explicit insertion_template_method(
            binary_search_tree<tkey, tvalue> *tree,
            typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy);
        
    public:

        void insert(
            tkey const &key,
            tvalue const &value);
        
        void insert(
            tkey const &key,
            tvalue &&value);
    
        void set_insertion_strategy(
            typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy) noexcept;
    
    protected:
        
        // TODO: think about it!
    
    private:
        
        [[nodiscard]] allocator *get_allocator() const noexcept final;
        
    };
    
    class obtaining_template_method:
        public template_method_basics
    {
    
    private:
        
        //binary_search_tree<tkey, tvalue> *_tree;
    
    public:
        
        explicit obtaining_template_method(
            binary_search_tree<tkey, tvalue> *tree);
    
    public:
        
        tvalue const &obtain(
            tkey const &key);
    
    protected:
        
        // TODO: think about it!
        
    };
    
    class disposal_template_method:
        public template_method_basics,
        public allocator_guardant
    {
    
    private:
        disposal_of_nonexistent_key_attempt_strategy _strategy; 
        node *_parent_node = nullptr;
        node *_cur_node = nullptr;

    public:
        
        explicit disposal_template_method(
            binary_search_tree<tkey, tvalue> *tree,
            typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy);
        
    public:
        tvalue dispose(
            tkey const &key);
        
        void set_disposal_strategy(
            typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) noexcept;

        disposal_of_nonexistent_key_attempt_strategy get_strategy() {return _strategy;};

        node * get_parent_node() {return _parent_node;};
        void set_parent_node(node *n) {_parent_node = n;};

        node * get_cur_node() {return _cur_node;};
        void set_cur_node(node *n) {_cur_node = n;};

    private:
        
        [[nodiscard]] allocator *get_allocator() const noexcept final;
        
    };

private:    
    node *_root = nullptr;
    
    insertion_template_method *_insertion_template;
    
    obtaining_template_method *_obtaining_template;
    
    disposal_template_method *_disposal_template;

protected:
    
    explicit binary_search_tree(
        typename binary_search_tree<tkey, tvalue>::insertion_template_method *insertion_template,
        typename binary_search_tree<tkey, tvalue>::obtaining_template_method *obtaining_template,
        typename binary_search_tree<tkey, tvalue>::disposal_template_method *disposal_template,
        std::function<int(tkey const &, tkey const &)>,
        allocator *allocator,
        logger *logger);

public:
    
    explicit binary_search_tree(
        std::function<int(tkey const &, tkey const &)> comparer = std::less<tkey>(),
        allocator *allocator = nullptr,
        logger *logger = nullptr,
        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy = binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::throw_an_exception,
        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy = binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::throw_an_exception);

private:

    // void destruct() noexcept
    // {
    //     deallocate_with_guard(reinterpret_cast<uint8_t *>(_));
    // }

    // //void copy_from(
    //     //
    // //)

    // void move_from(
    //     allocator_boundary_tags &&other)
    // {
    //     _trusted_memory = other._trusted_memory;
    //     other._trusted_memory = nullptr;
    // }

    iterator_data * create_iterator_data() const
    {
        void *memory = search_tree<tkey, tvalue>::allocate_with_guard(sizeof(iterator_data), 1);
        return new (memory)iterator_data();
    }

    node * create_node(tkey const & key, tvalue const & value) const
    {
        void *memory = search_tree<tkey, tvalue>::allocate_with_guard(sizeof(node), 1);
        return new (memory)node(key, value);
    }

    void deallocate_node(node * n)
    {
        search_tree<tkey, tvalue>::deallocate_with_guard(n);
    }

    void simple_delete_node(node * node_for_delete, node * parent_node)
    {
        if (parent_node == nullptr)
            _root = (node_for_delete->left_subtree == nullptr) ? node_for_delete->right_subtree : node_for_delete->left_subtree;
        else
        {
            if (!(is_templ_node(node_for_delete->left_subtree)))
            {
                if (parent_node->left_subtree == node_for_delete)
                    parent_node->left_subtree = node_for_delete->left_subtree;
                else
                    parent_node->right_subtree = node_for_delete->left_subtree;
            }
            else
            {
                if (parent_node->left_subtree == node_for_delete)
                    parent_node->left_subtree = node_for_delete->right_subtree;
                else
                    parent_node->right_subtree = node_for_delete->right_subtree;
            }
        }

        deallocate_node(node_for_delete);
    }

public:
    
    binary_search_tree(
        binary_search_tree<tkey, tvalue> const &other);
    
    binary_search_tree(
        binary_search_tree<tkey, tvalue> &&other) noexcept;
    
    binary_search_tree<tkey, tvalue> &operator=(
        binary_search_tree<tkey, tvalue> const &other);
    
    binary_search_tree<tkey, tvalue> &operator=(
        binary_search_tree<tkey, tvalue> &&other) noexcept;
    
    ~binary_search_tree() override;

private:

    void destroy_node(node *);
    
public:
    
    void insert(
        tkey const &key,
        tvalue const &value) final;
    
    void insert(
        tkey const &key,
        tvalue &&value) final;
    
    tvalue const &obtain(
        tkey const &key) final;
    
    std::vector<typename associative_container<tkey, tvalue>::key_value_pair> obtain_between(
        tkey const &lower_bound,
        tkey const &upper_bound,
        bool lower_bound_inclusive,
        bool upper_bound_inclusive) final;
    
    tvalue dispose(
        tkey const &key) final;
    
public:
    
    void set_insertion_strategy(
        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy) noexcept;
    
    void set_removal_strategy(
        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) noexcept;

public:
    
    // region iterators requests definition
    
    prefix_iterator begin_prefix() const noexcept;
    
    prefix_iterator end_prefix() const noexcept;
    
    prefix_const_iterator cbegin_prefix() const noexcept;
    
    prefix_const_iterator cend_prefix() const noexcept;
    
    prefix_reverse_iterator rbegin_prefix() const noexcept;
    
    prefix_reverse_iterator rend_prefix() const noexcept;
    
    prefix_const_reverse_iterator crbegin_prefix() const noexcept;
    
    prefix_const_reverse_iterator crend_prefix() const noexcept;
    
    infix_iterator begin_infix() const noexcept;
    
    infix_iterator end_infix() const noexcept;
    
    infix_const_iterator cbegin_infix() const noexcept;
    
    infix_const_iterator cend_infix() const noexcept;
    
    infix_reverse_iterator rbegin_infix() const noexcept;
    
    infix_reverse_iterator rend_infix() const noexcept;
    
    infix_const_reverse_iterator crbegin_infix() const noexcept;
    
    infix_const_reverse_iterator crend_infix() const noexcept;
    
    postfix_iterator begin_postfix() const noexcept;
    
    postfix_iterator end_postfix() const noexcept;
    
    postfix_const_iterator cbegin_postfix() const noexcept;
    
    postfix_const_iterator cend_postfix() const noexcept;
    
    postfix_reverse_iterator rbegin_postfix() const noexcept;
    
    postfix_reverse_iterator rend_postfix() const noexcept;
    
    postfix_const_reverse_iterator crbegin_postfix() const noexcept;
    
    postfix_const_reverse_iterator crend_postfix() const noexcept;
    
    // endregion iterators requests definition

protected:
    
    // region subtree rotations definition
    
    void small_left_rotation(
        typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool validate = true) const;
    
    void small_right_rotation(
        typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool validate = true) const;
    
    void big_left_rotation(
        typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool validate = true) const;
    
    void big_right_rotation(
        typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool validate = true) const;
    
    void double_left_rotation(
        typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool at_grandparent_first,
        bool validate = true) const;
    
    void double_right_rotation(
        typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool at_grandparent_first,
        bool validate = true) const;
    
    // endregion subtree rotations definition
    
};


template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::node::node(
    tkey const &key,
    tvalue const &value)
    : key(key)
    , value(value)
    , left_subtree(nullptr)
    , right_subtree(nullptr)
{
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::node::node(
    tkey const &key,
    tvalue &&value)
    : key(key)
    , value(value)
    , left_subtree(nullptr)
    , right_subtree(nullptr)
{
}


template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::iterator_data::iterator_data(
    unsigned int depth,
    tkey const &key,
    tvalue const &value):
    depth(depth),
    key(key),
    value(value)
{
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_iterator::prefix_iterator(
    typename binary_search_tree<tkey, tvalue>::node *subtree_root,
    binary_search_tree<tkey, tvalue> *tree)
    : iterator<prefix_iterator>(tree)
{
    this->_subtree_root = subtree_root;
    this->node_stack.push_back(subtree_root);
}

template<typename T>
T & prefix_plus_plus(T * rh)
{
    if (rh->get_subtree_root()->right_subtree != nullptr)
        rh->get_node_stack().push_back(rh->get_subtree_root()->right_subtree);

    if (rh->get_subtree_root()->left_subtree != nullptr)
        rh->set_subtree_root(rh->get_subtree_root()->left_subtree);
    else
    {
        if (!rh->get_node_stack().empty())
        {
            rh->set_subtree_root(*rh->get_node_stack().rbegin());
            rh->get_node_stack().pop_back();
        }
        else
            rh->set_subtree_root(nullptr);
    }

    return *rh;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_iterator &binary_search_tree<tkey, tvalue>::prefix_iterator::operator++()
{
    return prefix_plus_plus<binary_search_tree<tkey, tvalue>::prefix_iterator>(this);
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_iterator::prefix_const_iterator(
    typename binary_search_tree<tkey, tvalue>::node *subtree_root,
    binary_search_tree<tkey, tvalue> const * tree)
    : const_iterator<prefix_const_iterator>(tree)
{
    this->_subtree_root = subtree_root;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_iterator &binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator++()
{
    return prefix_plus_plus<binary_search_tree<tkey, tvalue>::prefix_const_iterator>(this);
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::prefix_reverse_iterator(
    typename binary_search_tree<tkey, tvalue>::node *subtree_root,
    binary_search_tree<tkey, tvalue> *tree)
    : iterator<prefix_reverse_iterator>(tree)
{
    this->_subtree_root = subtree_root;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator &binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator++()
{
    get_first_node();

    return *this;
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::prefix_const_reverse_iterator(
    typename binary_search_tree<tkey, tvalue>::node *subtree_root,
    binary_search_tree<tkey, tvalue> *tree)
    : iterator<prefix_const_reverse_iterator>(tree)
{
    this->_subtree_root = subtree_root;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator &binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator++()
{
    get_first_node();

    return *this;
}

template<typename T>
T & infix_plus_plus(T * rh)
{
    if (rh->get_node_stack().empty())
    {
        rh->set_subtree_root(nullptr);
        return *rh;
    }

    rh->get_node_stack().pop_back();

    if (rh->get_subtree_root()->right_subtree != nullptr)
    {
        rh->set_subtree_root(rh->get_subtree_root()->right_subtree);
        rh->get_left_leaf();
        return *rh;
    }

    rh->set_subtree_root(*(rh->get_node_stack().rbegin()));     
    return *rh;
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::infix_iterator::infix_iterator(
    typename binary_search_tree<tkey, tvalue>::node *subtree_root,
    binary_search_tree<tkey, tvalue> *tree)
    : const_iterator<infix_const_iterator>(tree)
{
    this->_subtree_root = subtree_root;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_iterator &binary_search_tree<tkey, tvalue>::infix_iterator::operator++()
{
    return prefix_plus_plus<binary_search_tree<tkey, tvalue>::infix_iterator>(this);
}


template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_iterator::infix_const_iterator(
    typename binary_search_tree<tkey, tvalue>::node *subtree_root,
    binary_search_tree<tkey, tvalue> const * tree)
    : const_iterator<infix_const_iterator>(tree)
{
    this->_subtree_root = subtree_root;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_iterator &binary_search_tree<tkey, tvalue>::infix_const_iterator::operator++()
{
    return infix_plus_plus<binary_search_tree<tkey, tvalue>::infix_const_iterator>(this);
}

// endregion infix_const_iterator implementation

// region infix_reverse_iterator implementation

template<typename T>
T & infix_rev_plus_plus(T * rh)
{
    if (rh->get_node_stack().empty())
    {
        rh->set_subtree_root(nullptr);
        return *rh;
    }

    rh->get_node_stack().pop_back();

    if (rh->get_subtree_root()->left_subtree != nullptr)
    {
        rh->set_subtree_root(rh->get_subtree_root()->left_subtree);
        rh->get_right_leaf();
        return *rh;
    }

    rh->set_subtree_root(*(rh->get_node_stack().rbegin()));
    return *rh;
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::infix_reverse_iterator::infix_reverse_iterator(
    typename binary_search_tree<tkey, tvalue>::node *subtree_root,
    binary_search_tree<tkey, tvalue> *tree)
    : const_iterator<infix_const_iterator>(tree)
{
    this->_subtree_root = subtree_root;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator &binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator++()
{
    return infix_rev_plus_plus<binary_search_tree<tkey, tvalue>::infix_reverse_iterator>(this);
}


template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::infix_const_reverse_iterator(
    typename binary_search_tree<tkey, tvalue>::node *subtree_root,
    binary_search_tree<tkey, tvalue> *tree)
    : const_iterator<infix_const_iterator>(tree)
{
    this->_subtree_root = subtree_root;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator &binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator++()
{
    return infix_rev_plus_plus<binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator>(this);
}

// endregion infix_const_reverse_iterator implementation

// region postfix_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_iterator::postfix_iterator(
    typename binary_search_tree<tkey, tvalue>::node *subtree_root,
    binary_search_tree<tkey, tvalue> const * tree)
    : iterator<postfix_iterator>(tree)
{
    this->_subtree_root = subtree_root;
}


template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_iterator &binary_search_tree<tkey, tvalue>::postfix_iterator::operator++()
{
    get_first_node();

    return *this;
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_iterator::postfix_const_iterator(
    typename binary_search_tree<tkey, tvalue>::node *subtree_root,
    binary_search_tree<tkey, tvalue> const * tree)
    : const_iterator<postfix_const_iterator>(tree)
{
    this->_subtree_root = subtree_root;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_iterator &binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator++()
{
    get_first_node();

    return *this;
}

template<typename T>
T & postfix_reverse_plus_plus(T * rh)
{
    if (rh->get_subtree_root()->left_subtree != nullptr)
        rh->get_node_stack().push_back(rh->get_subtree_root()->left_subtree);

    if (rh->get_subtree_root()->right_subtree != nullptr)
        rh->set_subtree_root(rh->get_subtree_root()->right_subtree);
    else
    {
        if (!rh->get_node_stack().empty())
        {
            rh->set_subtree_root(*rh->get_node_stack().rbegin());
            rh->get_node_stack().pop_back();
        }
        else
            rh->set_subtree_root(nullptr);
    }

    return *rh;
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::postfix_reverse_iterator(
    typename binary_search_tree<tkey, tvalue>::node *subtree_root,
    binary_search_tree<tkey, tvalue> *tree)
    : iterator<postfix_reverse_iterator>(tree)
{
    this->_subtree_root = subtree_root;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator &binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator++()
{
    return postfix_reverse_plus_plus<binary_search_tree<tkey, tvalue>::postfix_reverse_iterator>(this);
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::postfix_const_reverse_iterator(
    typename binary_search_tree<tkey, tvalue>::node *subtree_root,
    binary_search_tree<tkey, tvalue> *tree)
    : iterator<postfix_const_reverse_iterator>(tree)
{
    this->_subtree_root = subtree_root;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator &binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator++()
{
    return postfix_reverse_plus_plus<binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator>(this);
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_exception::insertion_of_existent_key_attempt_exception(
    tkey const &key):
    std::logic_error("Attempt to insert already existing key inside the tree.")
{
}

template<
    typename tkey,
    typename tvalue>
tkey const &binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_exception::get_key() const noexcept
{
    return _key;
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::obtaining_of_nonexistent_key_attempt_exception::obtaining_of_nonexistent_key_attempt_exception(
    tkey const &key):
    std::logic_error("Attempt to obtain a value by non-existing key from the tree.")
{

}

template<
    typename tkey,
    typename tvalue>
tkey const &binary_search_tree<tkey, tvalue>::obtaining_of_nonexistent_key_attempt_exception::get_key() const noexcept
{
    return _key;
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_exception::disposal_of_nonexistent_key_attempt_exception(
    tkey const &key):
    std::logic_error("Attempt to dispose a value by non-existing key from the tree.")
{

}

template<
    typename tkey,
    typename tvalue>
tkey const &binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_exception::get_key() const noexcept
{
    return _key;
}

// endregion target operations associated exception types implementation

// region template methods implementation

// region binary_search_tree<tkey, tvalue>::template_method_basics implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::template_method_basics::template_method_basics(
    binary_search_tree<tkey, tvalue> *tree)
    : _tree(tree)
{
}

template<
    typename tkey,
    typename tvalue>
[[nodiscard]] inline logger *binary_search_tree<tkey, tvalue>::template_method_basics::get_logger() const noexcept
{
    return _tree->get_logger();
}

// endregion binary_search_tree<tkey, tvalue>::template_method_basics implementation

// region search_tree<tkey, tvalue>::insertion_template_method implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::insertion_template_method::insertion_template_method(
    binary_search_tree<tkey, tvalue> *tree,
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy):
    binary_search_tree<tkey, tvalue>::template_method_basics::template_method_basics(tree)
{
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::insertion_template_method::insert(
    tkey const &key,
    tvalue const &value)
{
    throw not_implemented("template<typename tkey, typename tvalue> void binary_search_tree<tkey, tvalue>::insertion_template_method::insert(tkey const &, tvalue const &)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::is_templ_node(node const * test_node)
{
    return (test_node == nullptr);
}

template<
    typename tkey,
    typename tvalue>
unsigned int binary_search_tree<tkey, tvalue>::get_depth(node const * test_node) const
{
    unsigned int res_depth = 0;
    if (is_templ_node(test_node))
        return res_depth;

    binary_search_tree::node *cur_node = this->_root;

    auto comp_res = this->_keys_comparer(test_node->key, cur_node->key);
    while (true)
    {
        comp_res = this->_keys_comparer(test_node->key, cur_node->key);
        cur_node = (comp_res > 0) ? cur_node->right_subtree : cur_node->left_subtree;

        if (!(comp_res))
            break;
        res_depth += 1;
    }

    return res_depth;
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::insertion_template_method::insert(
    tkey const &key,
    tvalue &&value)
{
    binary_search_tree::node *prev_node = nullptr;
    binary_search_tree::node *cur_node = template_method_basics::_tree->_root;
    
    int comp_res;
    while (!(is_templ_node(cur_node)))
    {
        prev_node = cur_node;

        comp_res = template_method_basics::_tree->_keys_comparer(key, cur_node->key);

        if (comp_res == 0)
        {
            cur_node->value = value;
            return;
        }

        cur_node = (comp_res > 0) ? cur_node->right_subtree : cur_node->left_subtree;
    }

    if (prev_node == nullptr)
    {
        template_method_basics::_tree->_root = template_method_basics::_tree->create_node(key, value);
        return;
    }

    if (comp_res > 0)
        prev_node->right_subtree = template_method_basics::_tree->create_node(key, value);
    else 
        prev_node->left_subtree = template_method_basics::_tree->create_node(key, value);
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::insertion_template_method::set_insertion_strategy(
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue> void binary_search_tree<tkey, tvalue>::insertion_template_method::set_insertion_strategy(typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_strategy)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
allocator *binary_search_tree<tkey, tvalue>::insertion_template_method::get_allocator() const noexcept
{
    return template_method_basics::_tree->get_allocator();
    // throw not_implemented("template<typename tkey, typename tvalue> allocator *binary_search_tree<tkey, tvalue>::insertion_template_method::get_allocator() const noexcept", "your code should be here...");
}

// endregion search_tree<tkey, tvalue>::insertion_template_method implementation

// region search_tree<tkey, tvalue>::obtaining_template_method implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::obtaining_template_method::obtaining_template_method(
    binary_search_tree<tkey, tvalue> *tree):
    binary_search_tree<tkey, tvalue>::template_method_basics::template_method_basics(tree)
{
}

template<
    typename tkey,
    typename tvalue>
tvalue const &binary_search_tree<tkey, tvalue>::obtaining_template_method::obtain(
    tkey const &key)
{
    binary_search_tree::node *cur_node = template_method_basics::_tree->_root;
    
    auto comp_res = template_method_basics::_tree->_keys_comparer(key, cur_node->key);

    while ((cur_node != nullptr) &&
        (comp_res != 0))
    {
        cur_node = (comp_res > 0) ? cur_node->right_subtree : cur_node->left_subtree;
        comp_res = template_method_basics::_tree->_keys_comparer(key, cur_node->key);
    }

    return cur_node->value;
}

// endregion search_tree<tkey, tvalue>::obtaining_template_method implementation

// region search_tree<tkey, tvalue>::disposal_template_method implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::disposal_template_method::disposal_template_method(
    binary_search_tree<tkey, tvalue> *tree,
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy)
    : binary_search_tree<tkey, tvalue>::template_method_basics(tree)
    , _strategy(disposal_strategy)
{
}

template<
    typename tkey,
    typename tvalue>
tvalue binary_search_tree<tkey, tvalue>::disposal_template_method::dispose(
    tkey const &key)
{
    auto res = _cur_node->value;

    if ((!is_templ_node(_cur_node->right_subtree)) &&
        (!is_templ_node(_cur_node->left_subtree)))
    {
        node *parent_node_for_insert = nullptr;
        node *node_for_insert = _cur_node->left_subtree;

        while (!is_templ_node(node_for_insert->right_subtree))
        {
            parent_node_for_insert = node_for_insert;
            node_for_insert = node_for_insert->right_subtree;
        }

        _cur_node->key = node_for_insert->key;
        _cur_node->value = node_for_insert->value;

        template_method_basics::_tree->simple_delete_node(node_for_insert, parent_node_for_insert);
    }
    else
    {
        template_method_basics::_tree->simple_delete_node(_cur_node, _parent_node);
    }
    
    return res;
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::disposal_template_method::set_disposal_strategy(
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue> void binary_search_tree<tkey, tvalue>::disposal_template_method::set_disposal_strategy(typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_strategy)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
[[nodiscard]] inline allocator *binary_search_tree<tkey, tvalue>::disposal_template_method::get_allocator() const noexcept
{
    // throw not_implemented("template<typename tkey, typename tvalue> [[nodiscard]] inline allocator *binary_search_tree<tkey, tvalue>::disposal_template_method::get_allocator() const noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
    typename binary_search_tree<tkey, tvalue>::insertion_template_method *insertion_template,
    typename binary_search_tree<tkey, tvalue>::obtaining_template_method *obtaining_template,
    typename binary_search_tree<tkey, tvalue>::disposal_template_method *disposal_template,
    std::function<int(tkey const &, tkey const &)> comparer,
    allocator *allocator,
    logger *logger):
    search_tree<tkey, tvalue>(comparer, logger, allocator),
    _insertion_template(insertion_template),
    _obtaining_template(obtaining_template),
    _disposal_template(disposal_template)
{
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
    std::function<int(tkey const &, tkey const &)> keys_comparer,
    allocator *allocator,
    logger *logger,
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy,
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
    binary_search_tree(
        new binary_search_tree<tkey, tvalue>::insertion_template_method(this, insertion_strategy),
        new binary_search_tree<tkey, tvalue>::obtaining_template_method(this),
        new binary_search_tree<tkey, tvalue>::disposal_template_method(this, disposal_strategy),
        keys_comparer,
        allocator,
        logger)
{
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
    binary_search_tree<tkey, tvalue> const &other)
{
    throw not_implemented("template<typename tkey, typename tvalue> binary_search_tree<tkey, tvalue>::binary_search_tree(binary_search_tree<tkey, tvalue> const &)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
    binary_search_tree<tkey, tvalue> &&other) noexcept
    : search_tree<tkey, tvalue>(std::move(other))
    , _root(other._root)
{  
    other._root = nullptr;  
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue> &binary_search_tree<tkey, tvalue>::operator=(
    binary_search_tree<tkey, tvalue> const &other)
{
    throw not_implemented("template<typename tkey, typename tvalue> binary_search_tree<tkey, tvalue> &binary_search_tree<tkey, tvalue>::operator=(binary_search_tree<tkey, tvalue> const &)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue> &binary_search_tree<tkey, tvalue>::operator=(
    binary_search_tree<tkey, tvalue> &&other) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue> binary_search_tree<tkey, tvalue> &binary_search_tree<tkey, tvalue>::operator=(binary_search_tree<tkey, tvalue> &&) noexcept", "your code should be here...");
}


template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::~binary_search_tree()
{
    destroy_node(_root);
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::destroy_node(node * n)
{
    if (n == nullptr)
        return;

    destroy_node(n->left_subtree);
    destroy_node(n->right_subtree);

    binary_search_tree<tkey, tvalue>::deallocate_with_guard(n);
}

// endregion construction, assignment, destruction implementation

// region associative_container<tkey, tvalue> contract implementation

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::insert(
    tkey const &key,
    tvalue const &value)
{
    _insertion_template->insert(key, value);
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::insert(
    tkey const &key,
    tvalue &&value)
{
    _insertion_template->insert(key, std::move(value));
}

template<
    typename tkey,
    typename tvalue>
tvalue const &binary_search_tree<tkey, tvalue>::obtain(
    tkey const &key)
{
    return _obtaining_template->obtain(key);
}

template<
    typename tkey,
    typename tvalue>
std::vector<typename associative_container<tkey, tvalue>::key_value_pair> binary_search_tree<tkey, tvalue>::obtain_between(
    tkey const &lower_bound,
    tkey const &upper_bound,
    bool lower_bound_inclusive,
    bool upper_bound_inclusive)
{
    std::vector<typename associative_container<tkey, tvalue>::key_value_pair> res;

    auto it = cbegin_infix();    
    while ((*it)->key < lower_bound)
        ++it;

    if ((*it)->key == lower_bound && lower_bound_inclusive)
        res.push_back( {(*it)->key, (*it)->value} );
    
    while (true)
    {
        ++it;
        
        if (*it == nullptr)
            break;

        if ((*it)->key >= upper_bound)
        {
            if (upper_bound == (*it)->key && upper_bound_inclusive)
                res.push_back( {(*it)->key, (*it)->value} );

            break;
        }
        else
            res.push_back( {(*it)->key, (*it)->value} );
    }

    
    return res;
}

template<
    typename tkey,
    typename tvalue>
tvalue binary_search_tree<tkey, tvalue>::dispose(
    tkey const &key)
{
    node *parent_node = nullptr;
    node *cur_node = _root;
    
    auto comp_res = this->_keys_comparer(key, cur_node->key);

    while ((cur_node != nullptr) &&
        (comp_res != 0))
    {
        parent_node = cur_node;
        cur_node = (comp_res > 0) ? cur_node->right_subtree : cur_node->left_subtree;

        comp_res = this->_keys_comparer(key, cur_node->key);
    }

    _disposal_template->set_cur_node(cur_node);
    _disposal_template->set_parent_node(parent_node);

    if (_disposal_template->get_cur_node() == nullptr)
    {
        if (_disposal_template->get_strategy() == binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::do_nothing)
            return tvalue();
        else
        {
            throw new disposal_of_nonexistent_key_attempt_exception(key);
        }
    }

    return _disposal_template->dispose(key);
}


template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::set_insertion_strategy(
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue> void binary_search_tree<tkey, tvalue>::set_insertion_strategy(typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_strategy) noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::set_removal_strategy(
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue> void binary_search_tree<tkey, tvalue>::set_removal_strategy(typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_strategy) noexcept", "your code should be here...");
}

// region iterators requesting implementation

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_iterator binary_search_tree<tkey, tvalue>::begin_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_iterator(dynamic_cast<typename binary_search_tree<tkey, tvalue>::node *>(_root), this);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_iterator binary_search_tree<tkey, tvalue>::end_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_iterator(nullptr, this);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_iterator binary_search_tree<tkey, tvalue>::cbegin_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_const_iterator(dynamic_cast<typename binary_search_tree<tkey, tvalue>::node *>(_root), this);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_iterator binary_search_tree<tkey, tvalue>::cend_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_const_iterator(nullptr, this);
}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::set_node_stack(node * cur_node)
{
    this->node_stack.push_back(cur_node);

    if (!is_templ_node(cur_node->left_subtree))
        set_node_stack(cur_node->left_subtree);

    if (!is_templ_node(cur_node->right_subtree))
        set_node_stack(cur_node->right_subtree);
}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::get_first_node()
{
    if (this->node_stack.empty())
        this->_subtree_root = nullptr;
    else
    {
        this->_subtree_root = *(this->node_stack.rbegin());
        this->node_stack.pop_back();
    }
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator binary_search_tree<tkey, tvalue>::rbegin_prefix() const noexcept
{
    auto it = binary_search_tree<tkey, tvalue>::prefix_reverse_iterator(dynamic_cast<node *>(_root), this);
    it.set_node_stack(this->_root);
    it.get_first_node();

    return it;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator binary_search_tree<tkey, tvalue>::rend_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_reverse_iterator(nullptr, this);
}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::set_node_stack(node * cur_node)
{
    this->node_stack.push_back(cur_node);

    if (!is_templ_node(cur_node->left_subtree))
        set_node_stack(cur_node->left_subtree);

    if (!is_templ_node(cur_node->right_subtree))
        set_node_stack(cur_node->right_subtree);
}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::get_first_node()
{
    if (this->node_stack.empty())
        this->_subtree_root = nullptr;
    else
    {
        this->_subtree_root = *(this->node_stack.rbegin());
        this->node_stack.pop_back();
    }
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crbegin_prefix() const noexcept
{
    auto it = binary_search_tree<tkey, tvalue>::prefix_reverse_iterator(dynamic_cast<node *>(_root), this);
    it.set_node_stack(this->_root);
    it.get_first_node();

    return it;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crend_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator(nullptr, this);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_iterator binary_search_tree<tkey, tvalue>::begin_infix() const noexcept
{
    auto it = binary_search_tree<tkey, tvalue>::infix_const_iterator(dynamic_cast<typename binary_search_tree<tkey, tvalue>::node *>(_root), this);
    it.get_left_leaf();
    return it;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_iterator binary_search_tree<tkey, tvalue>::end_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_iterator(nullptr, this);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_iterator binary_search_tree<tkey, tvalue>::cbegin_infix() const noexcept
{
    auto it = binary_search_tree<tkey, tvalue>::infix_const_iterator(dynamic_cast<typename binary_search_tree<tkey, tvalue>::node *>(_root), this);
    it.get_left_leaf();
    return it;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_iterator binary_search_tree<tkey, tvalue>::cend_infix() const noexcept
{       
    return binary_search_tree<tkey, tvalue>::infix_const_iterator(nullptr, this);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator binary_search_tree<tkey, tvalue>::rbegin_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_reverse_iterator(dynamic_cast<typename binary_search_tree<tkey, tvalue>::node *>(_root), this);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator binary_search_tree<tkey, tvalue>::rend_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_reverse_iterator(nullptr, this);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crbegin_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator(dynamic_cast<typename binary_search_tree<tkey, tvalue>::node *>(_root), this);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crend_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator(nullptr, this);
}


template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_iterator binary_search_tree<tkey, tvalue>::begin_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_iterator(dynamic_cast<typename binary_search_tree<tkey, tvalue>::node *>(_root), this);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_iterator binary_search_tree<tkey, tvalue>::end_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_iterator(nullptr, this);
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::postfix_const_iterator::set_node_stack(node * cur_node)
{
    this->node_stack.push_back(cur_node);

    if (!is_templ_node(cur_node->right_subtree))
        set_node_stack(cur_node->right_subtree);

    if (!is_templ_node(cur_node->left_subtree))
        set_node_stack(cur_node->left_subtree);
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::postfix_const_iterator::get_first_node()
{
    if (this->node_stack.empty())
        this->_subtree_root = nullptr;
    else
    {
        this->_subtree_root = *(this->node_stack.rbegin());
        this->node_stack.pop_back();
    }
}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::postfix_iterator::set_node_stack(node * cur_node)
{
    this->node_stack.push_back(cur_node);

    if (!is_templ_node(cur_node->right_subtree))
        set_node_stack(cur_node->right_subtree);

    if (!is_templ_node(cur_node->left_subtree))
        set_node_stack(cur_node->left_subtree);
}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::postfix_iterator::get_first_node()
{
    if (this->node_stack.empty())
        this->_subtree_root = nullptr;
    else
    {
        this->_subtree_root = *(this->node_stack.rbegin());
        this->node_stack.pop_back();
    }
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_iterator binary_search_tree<tkey, tvalue>::cbegin_postfix() const noexcept
{
    auto it = binary_search_tree<tkey, tvalue>::postfix_const_iterator(dynamic_cast<typename binary_search_tree<tkey, tvalue>::node *>(_root), this);
    it.set_node_stack(this->_root);
    it.get_first_node();

    return it;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_iterator binary_search_tree<tkey, tvalue>::cend_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_const_iterator(nullptr, this);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator binary_search_tree<tkey, tvalue>::rbegin_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_reverse_iterator(dynamic_cast<typename binary_search_tree<tkey, tvalue>::node *>(_root), this);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator binary_search_tree<tkey, tvalue>::rend_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_reverse_iterator(nullptr, this);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crbegin_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator(dynamic_cast<typename binary_search_tree<tkey, tvalue>::node *>(_root), this);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crend_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator(nullptr, this);
}


// endregion iterators request implementation

// region subtree rotations implementation

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::small_left_rotation(
    binary_search_tree<tkey, tvalue>::node *&subtree_root,
    bool validate) const
{
    throw not_implemented("template<typename tkey, typename tvalue> void binary_search_tree<tkey, tvalue>::small_left_rotation(binary_search_tree<tkey, tvalue>::node *&, bool) const", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::small_right_rotation(
    binary_search_tree<tkey, tvalue>::node *&subtree_root,
    bool validate) const
{
    throw not_implemented("template<typename tkey, typename tvalue> void binary_search_tree<tkey, tvalue>::small_right_rotation(binary_search_tree<tkey, tvalue>::node *&, bool) const", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::big_left_rotation(
    binary_search_tree<tkey, tvalue>::node *&subtree_root,
    bool validate) const
{
    throw not_implemented("template<typename tkey, typename tvalue> void binary_search_tree<tkey, tvalue>::big_left_rotation(binary_search_tree<tkey, tvalue>::node *&, bool) const", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::big_right_rotation(
    binary_search_tree<tkey, tvalue>::node *&subtree_root,
    bool validate) const
{
    throw not_implemented("template<typename tkey, typename tvalue> void binary_search_tree<tkey, tvalue>::big_right_rotation(binary_search_tree<tkey, tvalue>::node *&, bool) const", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::double_left_rotation(
    binary_search_tree<tkey, tvalue>::node *&subtree_root,
    bool at_grandparent_first,
    bool validate) const
{
    throw not_implemented("template<typename tkey, typename tvalue> void binary_search_tree<tkey, tvalue>::double_left_rotation(binary_search_tree<tkey, tvalue>::node *&, bool, bool) const", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::double_right_rotation(
    binary_search_tree<tkey, tvalue>::node *&subtree_root,
    bool at_grandparent_first,
    bool validate) const
{
    throw not_implemented("template<typename tkey, typename tvalue> void binary_search_tree<tkey, tvalue>::double_right_rotation(binary_search_tree<tkey, tvalue>::node *&, bool, bool) const", "your code should be here...");
}


#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_BINARY_SEARCH_TREE_H
