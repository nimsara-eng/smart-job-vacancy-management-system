import { defineStore } from 'pinia'
import { ref, computed } from 'vue'
import type { User } from '../types'

export const useAuthStore = defineStore('auth', () => {
  const user = ref<User | null>(null)
  const isLoggedIn = computed(() => user.value !== null)

  function login(u: User) {
    user.value = u
    localStorage.setItem('sjvms_user', JSON.stringify(u))
  }

  function logout() {
    user.value = null
    localStorage.removeItem('sjvms_user')
  }

  function restoreSession() {
    const stored = localStorage.getItem('sjvms_user')
    if (stored) {
      try {
        user.value = JSON.parse(stored)
      } catch {
        user.value = null
      }
    }
  }

  return { user, isLoggedIn, login, logout, restoreSession }
})