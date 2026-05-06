import { defineStore } from 'pinia'
import type { User, AuthState } from '../types'

// Fixed: Converted to Options API. This guarantees TypeScript correctly infers 'user' as 'User | null' instead of 'Ref<User | null>'
export const useAuthStore = defineStore('auth', {
  state: (): AuthState => ({
    user: null,
    isLoggedIn: false
  }),
  actions: {
    login(u: User) {
      this.user = u
      this.isLoggedIn = true
      localStorage.setItem('sjvms_user', JSON.stringify(u))
    },
    logout() {
      this.user = null
      this.isLoggedIn = false
      localStorage.removeItem('sjvms_user')
    },
    restoreSession() {
      const stored = localStorage.getItem('sjvms_user')
      if (stored) {
        try {
          this.user = JSON.parse(stored)
          this.isLoggedIn = true
        } catch {
          this.user = null
          this.isLoggedIn = false
        }
      }
    }
  }
})