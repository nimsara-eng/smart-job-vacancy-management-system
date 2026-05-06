<template>
  <div class="auth-page">
    <div class="auth-card card">
      <h1 class="auth-title">Create account</h1>
      <p class="auth-sub">Join SJVMS as an applicant or employer</p>

      <!-- Role toggle -->
      <div class="role-toggle">
        <button
          v-for="r in roles"
          :key="r.value"
          type="button"
          class="role-btn"
          :class="{ active: form.role === r.value }"
          @click="form.role = r.value"
        >
          <span v-html="r.icon"/>
          {{ r.label }}
        </button>
      </div>

      <Transition name="fade">
        <div v-if="error" class="alert alert-error">{{ error }}</div>
      </Transition>
      <Transition name="fade">
        <div v-if="success" class="alert alert-success">Account created! Redirecting…</div>
      </Transition>

      <form class="auth-form" @submit.prevent="handleRegister">
        <div class="form-row">
          <div class="form-group">
            <label class="form-label">Full Name</label>
            <input v-model="form.name" class="form-input" placeholder="John Doe" required />
          </div>
          <div class="form-group">
            <label class="form-label">Username</label>
            <input v-model="form.username" class="form-input" placeholder="johndoe" required />
          </div>
        </div>
        <div class="form-group">
          <label class="form-label">Email</label>
          <input v-model="form.email" class="form-input" type="email" placeholder="john@example.com" required />
        </div>
        <div class="form-row">
          <div class="form-group">
            <label class="form-label">Phone</label>
            <input v-model="form.phone" class="form-input" placeholder="07X XXXXXXX" />
          </div>
          <div class="form-group">
            <label class="form-label">Password</label>
            <input v-model="form.password" class="form-input" type="password" placeholder="••••••••" required />
          </div>
        </div>
        <div v-if="form.role === 'applicant'" class="form-group">
          <label class="form-label">Your Skills</label>
          <SkillsInput v-model="form.skills" placeholder="e.g. Python, React…" />
        </div>
        <button type="submit" class="btn btn-primary btn-lg auth-btn" :disabled="loading">
          <span v-if="loading" class="spinner"/>
          {{ loading ? 'Creating account…' : 'Create Account' }}
        </button>
      </form>

      <p class="auth-switch">
        Already have an account? <RouterLink to="/login">Sign in →</RouterLink>
      </p>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, reactive } from 'vue'
import { useRouter } from 'vue-router'
import { useAuthStore } from '../stores/auth'
import { useDataStore } from '../stores/data'
import SkillsInput from '../components/SkillsInput.vue'

type Role = 'applicant' | 'employer'

const auth = useAuthStore()
const data = useDataStore()
const router = useRouter()

const form = reactive({
  name: '',
  username: '',
  email: '',
  phone: '',
  password: '',
  role: 'applicant' as Role,
  skills: [] as string[],
})

// ← explicit type so r.value is Role, not string
const roles: { value: Role; label: string; icon: string }[] = [
  { value: 'applicant', label: 'Job Seeker', icon: '<svg width="14" height="14" viewBox="0 0 24 24" fill="none"><circle cx="12" cy="8" r="4" stroke="currentColor" stroke-width="1.8"/><path d="M4 20c0-4 3.6-7 8-7s8 3 8 7" stroke="currentColor" stroke-width="1.8" stroke-linecap="round"/></svg>' },
  { value: 'employer',  label: 'Employer',   icon: '<svg width="14" height="14" viewBox="0 0 24 24" fill="none"><rect x="2" y="7" width="20" height="14" rx="2" stroke="currentColor" stroke-width="1.8"/><path d="M16 7V5a2 2 0 00-2-2h-4a2 2 0 00-2 2v2" stroke="currentColor" stroke-width="1.8"/></svg>' },
]

const error = ref('')
const success = ref(false)
const loading = ref(false)

async function handleRegister() {
  error.value = ''
  loading.value = true
  await new Promise(r => setTimeout(r, 400))
  try {
    const newUser = data.registerUser({
      username: form.username,
      password: form.password,
      email: form.email,
      name: form.name,
      phone: form.phone,
      role: form.role,
      skills: form.skills,
    })
    auth.login(newUser)
    success.value = true
    setTimeout(() => router.push('/dashboard'), 1200)
  } catch (e: any) {
    error.value = e.message
  } finally {
    loading.value = false
  }
}
</script>

<style scoped>
.auth-page {
  min-height: 100vh;
  display: grid;
  place-items: center;
  padding: 24px;
  background: radial-gradient(ellipse 60% 50% at 50% 0%, rgba(124,90,247,.06) 0%, transparent 70%);
}
.auth-card {
  width: 100%;
  max-width: 520px;
  padding: 40px;
  display: flex;
  flex-direction: column;
  gap: 16px;
}
.auth-title { font-size: 1.6rem; }
.auth-sub { font-size: .9rem; color: var(--c-text2); margin-top: -8px; }

.role-toggle { display: flex; gap: 8px; }
.role-btn {
  flex: 1;
  display: flex;
  align-items: center;
  justify-content: center;
  gap: 8px;
  padding: 12px;
  border-radius: var(--radius-sm);
  border: 1px solid var(--c-border);
  background: var(--c-surface2);
  color: var(--c-text2);
  font-size: .88rem;
  font-weight: 500;
  cursor: pointer;
  transition: all .15s;
}
.role-btn.active {
  border-color: var(--c-accent);
  color: var(--c-accent);
  background: rgba(79,142,247,.08);
}

.auth-form { display: flex; flex-direction: column; gap: 14px; }
.form-row { display: grid; grid-template-columns: 1fr 1fr; gap: 12px; }
.auth-btn { width: 100%; justify-content: center; }
.auth-switch { font-size: .85rem; color: var(--c-text2); text-align: center; }
.auth-switch a { color: var(--c-accent); font-weight: 500; }

.spinner {
  width: 14px; height: 14px;
  border: 2px solid rgba(255,255,255,.3);
  border-top-color: #fff;
  border-radius: 50%;
  animation: spin .6s linear infinite;
  display: inline-block;
}
@keyframes spin { to { transform: rotate(360deg); } }
</style>