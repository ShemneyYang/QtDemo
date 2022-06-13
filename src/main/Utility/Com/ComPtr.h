#pragma once

namespace Utility
{

	template<class T> class ComPtr {

	protected:
		T* ptr;

		inline void Kill()
		{
			if (ptr)
				ptr->Release();
		}

		inline void Replace(T* p)
		{
			if (ptr != p) {
				if (p)   p->AddRef();
				if (ptr) ptr->Release();
				ptr = p;
			}
		}

	public:
		inline ComPtr() : ptr(NULL) {}
		inline ComPtr(T* p) : ptr(p) { if (ptr) ptr->AddRef(); }
		inline ComPtr(const ComPtr<T>& c) : ptr(c.ptr) { if (ptr) ptr->AddRef(); }
#if _MSC_VER >= 1900
		inline ComPtr(ComPtr<T>&& c) : ptr(c.ptr) { c.ptr = NULL; }
#endif
		inline ~ComPtr() { Kill(); }

		inline void Clear()
		{
			if (ptr) {
				ptr->Release();
				ptr = NULL;
			}
		}

		inline ComPtr<T>& operator=(T* p)
		{
			Replace(p);
			return *this;
		}

		inline ComPtr<T>& operator=(const ComPtr<T>& c)
		{
			Replace(c.ptr);
			return *this;
		}

#if _MSC_VER >= 1900
		inline ComPtr<T>& operator=(ComPtr<T>&& c)
		{
			if (this != &c) {
				Kill();
				ptr = c.ptr;
				c.ptr = NULL;
			}

			return *this;
		}
#endif

		inline T* Detach()
		{
			T* out = ptr;
			ptr = NULL;
			return out;
		}

		inline void CopyTo(T** out)
		{
			if (out) {
				if (ptr) ptr->AddRef();
				*out = ptr;
			}
		}

		inline ULONG Release()
		{
			ULONG ref;

			if (!ptr) return 0;
			ref = ptr->Release();
			ptr = NULL;
			return ref;
		}

		inline T** Assign() { Clear(); return &ptr; }
		inline void Set(T* p) { Kill(); ptr = p; }

		inline T* Get() const { return ptr; }

		inline T** operator&() { return Assign(); }

		inline    operator T* () const { return ptr; }
		inline T* operator->() const { return ptr; }

		inline bool operator==(T* p) const { return ptr == p; }
		inline bool operator!=(T* p) const { return ptr != p; }

		inline bool operator!() const { return !ptr; }
	};

#ifdef _WIN32

	template<class T> class ComQIPtr : public ComPtr<T> {

	public:
		inline ComQIPtr(IUnknown* unk)
		{
			this->ptr = nullptr;
			unk->QueryInterface(__uuidof(T), (void**)&this->ptr);
		}

		inline ComPtr<T>& operator=(IUnknown* unk)
		{
			ComPtr<T>::Clear();
			unk->QueryInterface(__uuidof(T), (void**)&this->ptr);
			return *this;
		}
	};
#endif
}
