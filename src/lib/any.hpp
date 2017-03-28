#pragma once

#include <algorithm>
#include <typeinfo>

/**
 * Represents Any object.
 *
 * It wraps arbitrary object into @ref placeholder.
 * Which is abstract class that has virtual methods `.type()` and `.value()`.
 * Actual type information is stored in derived @ref impl class.
 * Using this information you can cast to required type correctly and safely.
 */
class Any {
    public:
        /**
         * Default empty instance.
         */
        Any() noexcept(true) : inner(nullptr) {};

        /**
         * Creates Any instance from value.
         */
        template<typename T>
        Any(const T &value) : inner(new impl<T>(value)) {};

        /**
         * Creates Any instance by cloning existing one.
         */
        Any(const Any &other) : inner(other.inner ? other.inner->clone() : nullptr) {};

        /**
         * Move constructor
         */
        Any(Any&& other) noexcept(true) : inner(other.inner) {
            other.inner = nullptr;
        };

        ~Any() {
            if (inner) delete inner;
        };

        Any& operator=(const Any &rhs) = delete;
        //Allow only move assignment.
        //It helps to sort and it makes no sense to copy around objects implicitly.
        Any& operator=(Any&& right) {
            if (this->inner) delete this->inner;

            this->inner = right.inner;
            right.inner = nullptr;

            return *this;
        }

        /**
         * Abstract placeholder for any value.
         * Just Abstract class with empty virtual methods.
         *
         * Idea from http://www.two-sdg.demon.co.uk/curbralan/papers/ValuedConversions.pdf
         */
        class placeholder {
            public:
                virtual ~placeholder() {};
                virtual const std::type_info& type() const noexcept(true) = 0;
                virtual placeholder* clone() const = 0;
        };

        /**
         * Implementation of @ref placeholder
         */
        template<typename ValueType>
        class impl: public placeholder {
            public:
                impl(const ValueType &value) noexcept(true) : value(value) {};

                virtual const std::type_info& type() const noexcept(true) {
                    return typeid(ValueType);
                }

                virtual placeholder* clone() const {
                    return new impl(value);
                }

                ValueType value;
        };

        operator bool() const {
            return inner != nullptr;
        }

        /**
         * @returns type of @ref Any.
         */
        inline const std::type_info& type() const noexcept(true) {
            return inner ? inner->type() : typeid(void);
        }

        /**
         * @returns pointer to value of @ref Any.
         * @retval nullptr On invalid type.
         */
        template<typename T>
        const T* value_ptr() const noexcept(true) {
            return this->type() == typeid(T) ? &static_cast<impl<T>*>(inner)->value : nullptr;
        }

        /**
         * @returns value of @ref Any.
         * @throws On invalid type.
         */
        template<typename T>
        T value() const {
            const T* ret_val = value_ptr<T>();
            return ret_val ? *ret_val : throw std::bad_cast();
        }

    private:
        placeholder* inner;
};
