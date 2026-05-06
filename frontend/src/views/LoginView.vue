<template>
  <div class="auth-page">
    <div class="auth-card card">
      <!-- Logo -->
      <div class="auth-logo">
        <svg width="28" height="28" viewBox="0 0 24 24" fill="none">
          <path d="M20 7H4C2.9 7 2 7.9 2 9v12c0 1.1.9 2 2 2h16c1.1 0 2-.9 2-2V9c0-1.1-.9-2-2-2z"
            stroke="var(--c-accent)" stroke-width="1.8" fill="none"/>
          <path d="M16 7V5c0-1.1-.9-2-2-2h-4c-1.1 0-2 .9-2 2v2" stroke="var(--c-accent)" stroke-width="1.8" fill="none"/>
        </svg>
      </div>
      <h1 class="auth-title">Welcome back</h1>
      <p class="auth-sub">Sign in to your SJVMS account</p>

      <Transition name="fade">
        <div v-if="error" class="alert alert-error">{{ error }}</div>
      </Transition>

      <form class="auth-form" @submit.prevent="handleLogin">
        <div class="form-group">
          <label class="form-label">Username</label>
          <input v-model="form.username" class="form-input" placeholder="Enter your username" required autocomplete="username" />
        </div>
        <div class="form-group">
          <label class="form-label">Password</label>
          <div class="input-pw">
            <input
              v-model="form.password"
              :type="showPw ? 'text' : 'password'"
              class="form-input"
              placeholder="Enter your password"
              required
              autocomplete="current-password"
            />
            <button type="button" class="pw-toggle" @click="showPw = !showPw">
              <svg v-if="!showPw" width="16" height="16" viewBox="0 0 24 24" fill="none"><path d="M1 12s4-8 11-8 11 8 11 8-4 8-11 8-11-8-11-8z" stroke="currentColor" stroke-width="1.8"/><circle cx="12" cy="12" r="3" stroke="currentColor" stroke-width="1.8"/></svg>
              <svg v-else width="16" height="16" viewBox="0 0 24 24" fill="none"><path d="M17.94 17.94A10.07 10.07 0 0112 20c-7 0-11-8-11-8a18.45 18.45 0 015.06-5.94M9.9 4.24A9.12 9.12 0 0112 4c7 0 11 8 11 8a18.5 18.5 0 01-2.16 3.19m-6.72-1.07a3 3 0 11-4.24-4.24" stroke="currentColor" stroke-width="1.8"/><line x1="1" y1="1" x2="23" y2="23" stroke="currentColor" stroke-width="1.8"/></svg>
            </button>
          </div>
        </div>

        <button type="submit" class="btn btn-primary btn-lg auth-btn" :disabled="loading">
          <span v-if="loading" class="spinner"/>
          {{ loading ? 'Signing in…' : 'Sign In' }}
        </button>
      </form>

      <p class="auth-switch">
        Don't have an account?
        <RouterLink to="/register">Create one →</RouterLink>
      </p>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, reactive } from 'vue'
import { useRouter, useRoute } from 'vue-router'
import { useAuthStore } from '../stores/auth'
import { useDataStore } from '../stores/data'

const auth = useAuthStore()
const data = useDataStore()
const router = useRouter()
const route = useRoute()

const form = reactive({ username: '', password: '' })
const error = ref('')
const loading = ref(false)
const showPw = ref(false)

async function handleLogin() {
  error.value = ''
  loading.value = true
  await new Promise(r => setTimeout(r, 400))
  const user = data.loginUser(form.username, form.password)
  loading.value = false
  if (!user) {
    error.value = 'Invalid username or password.'
    return
  }
  auth.login(user)
  const redirect = route.query.redirect as string | undefined
  router.push(redirect || '/dashboard')
}
</script>

<style scoped>
.auth-page {
  min-height: 100vh;
  display: grid;
  place-items: center;
  padding: 24px;
  background: radial-gradient(ellipse 60% 50% at 50% 0%, rgba(79,142,247,.07) 0%, transparent 70%);
}
.auth-card {
  width: 100%;
  max-width: 420px;
  padding: 40px;
  display: flex;
  flex-direction: column;
  gap: 16px;
}
.auth-logo {
  width: 52px;
  height: 52px;
  background: rgba(79,142,247,.1);
  border: 1px solid rgba(79,142,247,.2);
  border-radius: 14px;
  display: grid;
  place-items: center;
  margin-bottom: 4px;
}
.auth-title { font-size: 1.6rem; }
.auth-sub { font-size: .9rem; color: var(--c-text2); margin-top: -8px; }
.auth-form { display: flex; flex-direction: column; gap: 16px; margin-top: 4px; }
.auth-btn { width: 100%; justify-content: center; }
.auth-switch { font-size: .85rem; color: var(--c-text2); text-align: center; }
.auth-switch a { color: var(--c-accent); font-weight: 500; }

.input-pw { position: relative; }
.input-pw .form-input { padding-right: 42px; }
.pw-toggle {
  position: absolute;
  right: 12px;
  top: 50%;
  transform: translateY(-50%);
  background: none;
  border: none;
  cursor: pointer;
  color: var(--c-text3);
  display: grid;
  place-items: center;
  padding: 0;
}
.pw-toggle:hover { color: var(--c-text2); }

.spinner {
  width: 14px;
  height: 14px;
  border: 2px solid rgba(255,255,255,.3);
  border-top-color: #fff;
  border-radius: 50%;
  animation: spin .6s linear infinite;
  display: inline-block;
}
@keyframes spin { to { transform: rotate(360deg); } }
</style>